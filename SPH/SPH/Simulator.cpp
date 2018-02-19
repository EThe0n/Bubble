#include "Simulator.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

Simulator::Simulator()
{
	kernel			= CELL_SIZE;
	particleMass	= PARTICLE_MASS;
	particleNumber	= PARTICLE_NUMBER;
	radius			= PARTICLE_RADIUS;
	particleSize	= radius * 2.5f;

	worldSize.x		= CELL_SIZE * GRID_X_COUNT;
	worldSize.y		= CELL_SIZE * GRID_Y_COUNT;
	cellSize		= CELL_SIZE;
	cellNumber.x	= GRID_X_COUNT;
	cellNumber.y	= GRID_Y_COUNT;
	cellTotalNumber = cellNumber.x * cellNumber.y;

	gravity.x		= 0.0f;
	gravity.y		= GRAVITY;
	wallDamping		= WALL_DAMPING;
	restDensity		= PARTICLE_REST_DENSITY;
	gasStiffness	= GAS_STIFFNESS;
	viscosity		= VISCOSITY;
	surfaceNormal	= THRESHOLD;

	surfaceCoef		= SURFACE_TENSION;
	poly6Coef		= 315.0f / (64.0f * (float)M_PI * powf(kernel, 9.0f));
	spikyCoef		= -45.0f / ((float)M_PI * powf(kernel, 6.0f));
	viscosityCoef	= 45.0f / ((float)M_PI * powf(kernel, 6.0f));

	gradPoly6		= -945.0f / (32.0f * (float)M_PI * powf(kernel, 9.0f));
	laplacePoly6	= -945.0f / (8.0f * (float)M_PI * powf(kernel, 9.0f));

	kernel_2		= kernel * kernel;
	selfDensity		= particleMass * poly6Coef * powf(kernel, 6.0f);
	selfLaplaceColor = laplacePoly6 * particleMass * kernel_2 * (-0.75 * kernel_2);

	particles = new Particle(particleNumber);
	particles->initSorted(worldSize, radius, particleNumber);
	linkedCell = new LinkedCell(particleNumber, cellSize, cellNumber.x, cellNumber.y);
	grid = new Grid(cellNumber.x, cellNumber.y, cellSize);
}

Simulator::~Simulator()
{
	delete particles;
	delete linkedCell;
}

void Simulator::update()
{
	particles->update(particleNumber);
	linkedCell->update(particles->position);
}

void Simulator::simulate(float deltaTime)
{
	computeDensityAndPressure();
	computeForces();
	advection(deltaTime);
}

void Simulator::render(GLuint channel, GLuint particleSize)
{
	glUniform1f(particleSize, this->particleSize);

	glUniform1i(channel, PARTICLE_CHANNEL);
	particles->render(particleNumber);

	glUniform1i(channel, GRID_CHANNEL);
	grid->render();
}

void Simulator::computeDensityAndPressure()
{
	register int neighbor[5];
	register int neighborCount;
	register int start, end;
	register int neighborStart, neighborEnd;
	register int pIdx;
	float* den = particles->density;
	float* pres = particles->pressure;
	float* pos = particles->position;

	memset(den , 0, sizeof(float) * particleNumber);

	for (register int cellIdx = 0; cellIdx < cellTotalNumber; ++cellIdx) {
		start = linkedCell->cellStartIndex[cellIdx];
		end = start + linkedCell->particleNumberInCell[cellIdx];

		neighborCount = linkedCell->neighborCellSize[cellIdx];
		memcpy(neighbor, linkedCell->neighborCellIndex[cellIdx], sizeof(int) * neighborCount);

		for (register int i = start; i < end; ++i) {
			pIdx = linkedCell->particleIndex[i];

			// same cell collision check
			for (register int j = i + 1; j < end; ++j) {
				computeParticleDensity(den, pos, pIdx, linkedCell->particleIndex[j]);
			}

			// neighbor cells collision check 
			for (register int n = 0; n < neighborCount; ++n) {
				neighborStart = linkedCell->cellStartIndex[neighbor[n]];
				neighborEnd = neighborStart + linkedCell->particleNumberInCell[neighbor[n]];
				for (register int j = neighborStart; j < neighborEnd; ++j) {
					computeParticleDensity(den, pos, pIdx, linkedCell->particleIndex[j]);
				}
			}

			den[pIdx] += selfDensity;
			pres[pIdx] = (powf(den[pIdx] / restDensity, 7.0f) - 1.0f) * gasStiffness;
		}
	}
}

void Simulator::computeParticleDensity(float * den, float * pos, int lhs, int rhs)
{
	register int lhs_x = lhs* 2;
	register int lhs_y = lhs_x + 1;
	register int rhs_x = rhs * 2;
	register int rhs_y = rhs_x + 1;

	register float dx, dy;
	dx = pos[rhs_x] - pos[lhs_x];
	dy = pos[rhs_y] - pos[lhs_y];

	float r2 = dx * dx + dy * dy;

	if (isnan(r2) || r2 >= kernel_2 || r2 < INF) {
		return;
	}

	float cont = particleMass * poly6Coef * powf(kernel_2 - r2, 3.0f);

	den[lhs] += cont;
	den[rhs] += cont;
}

void Simulator::computeForces()
{
	register int neighbor[5];
	register int neighborCount;
	register int start, end;
	register int neighborStart, neighborEnd;
	register int pIdx, x, y;
	const int bytes = sizeof(float) * particleNumber;
	const int bytes_2 = bytes * 2;

	float* grad = particles->gradientColor;
	float* lapl = particles->laplaceColor;

	memset(particles->accel, 0, bytes_2);
	memset(particles->gradientColor, 0, bytes_2);
	memset(particles->laplaceColor, 0, bytes);

	for (register int cellIdx = 0; cellIdx < cellTotalNumber; ++cellIdx) {
		start = linkedCell->cellStartIndex[cellIdx];
		end = start + linkedCell->particleNumberInCell[cellIdx];

		neighborCount = linkedCell->neighborCellSize[cellIdx];
		memcpy(neighbor, linkedCell->neighborCellIndex[cellIdx], sizeof(int) * neighborCount);

		for (register int i = start; i < end; ++i) {
			pIdx = linkedCell->particleIndex[i];
			x = pIdx * 2;
			y = x + 1;

			// same cell collision check
			for (register int j = i + 1; j < end; ++j) {
				computeParticleForces(pIdx, linkedCell->particleIndex[j]);
			}

			// neighbor cells collision check 
			for (register int n = 0; n < neighborCount; ++n) {
				neighborStart = linkedCell->cellStartIndex[neighbor[n]];
				neighborEnd = neighborStart + linkedCell->particleNumberInCell[neighbor[n]];
				for (register int j = neighborStart; j < neighborEnd; ++j) {
					computeParticleForces(pIdx, linkedCell->particleIndex[j]);
				}
			}

			lapl[pIdx] += selfLaplaceColor / particles->density[pIdx];
			particles->surfaceNormal[pIdx] = sqrt(grad[x] * grad[x] + grad[y] * grad[y]);

			if (particles->surfaceNormal[pIdx] > surfaceNormal) {
				particles->accel[x] += surfaceCoef * lapl[pIdx] * grad[x] / particles->surfaceNormal[pIdx];
				particles->accel[y] += surfaceCoef * lapl[pIdx] * grad[y] / particles->surfaceNormal[pIdx];
			}
		}
	}
}

void Simulator::computeParticleForces(int lhs, int rhs)
{
	register int lhs_x = lhs * 2;
	register int lhs_y = lhs_x + 1;
	register int rhs_x = rhs * 2;
	register int rhs_y = rhs_x + 1;

	float dx = particles->position[lhs_x] - particles->position[rhs_x];
	float dy = particles->position[lhs_y] - particles->position[rhs_y];
	float r2 = dx * dx + dy * dy;

	if (r2 <= INF || r2 >= kernel_2) {
		return;
	}

	float r = sqrt(r2);
	float rv = 0.5f * particleMass / particles->density[rhs];
	float lv = 0.5f * particleMass / particles->density[lhs];
	float kernel_r = kernel - r;

	// pressure force
	float pressureKernel = spikyCoef * kernel_r * kernel_r;
	float rtemp = rv * (particles->pressure[lhs] + particles->pressure[rhs]) * pressureKernel;
	float ltemp = lv * (particles->pressure[lhs] + particles->pressure[rhs]) * pressureKernel;

	float* acc = particles->accel;
	acc[lhs_x] -= dx * rtemp / r;
	acc[lhs_y] -= dy * rtemp / r;
	acc[rhs_x] += dx * ltemp / r;
	acc[rhs_y] += dy * ltemp / r;

	// viscosity force
	float dvx = particles->ev[rhs_x] - particles->ev[lhs_x];
	float dvy = particles->ev[rhs_y] - particles->ev[lhs_y];
	float viscosityKernel = viscosityCoef * (kernel - r);
	rtemp = rv * viscosity * viscosityKernel;
	ltemp = lv * viscosity * viscosityKernel;
	acc[lhs_x] += dvx * rtemp;
	acc[lhs_y] += dvy * rtemp;
	acc[rhs_x] -= dvx * ltemp;
	acc[rhs_y] -= dvy * ltemp;

	rtemp = -1.0f * gradPoly6 * rv * powf(kernel_2 - r2, 2.0f);
	ltemp = -1.0f * gradPoly6 * lv * powf(kernel_2 - r2, 2.0f);

	particles->gradientColor[lhs_x] += rtemp * dx;
	particles->gradientColor[lhs_y] += rtemp * dy;
	particles->gradientColor[rhs_x] -= ltemp * dx;
	particles->gradientColor[rhs_y] -= ltemp * dy;

	particles->laplaceColor[lhs] += laplacePoly6 * rv * (kernel_2 - r2) * (r2 - 0.75f * (kernel_2 - r2));
	particles->laplaceColor[rhs] += laplacePoly6 * lv * (kernel_2 - r2) * (r2 - 0.75f * (kernel_2 - r2));
}

void Simulator::advection(float deltaTime)
{
	register int x, y;

	float* pos = particles->position;
	float* acc = particles->accel;
	float* vel = particles->velocity;
	float* den = particles->density;
	float* ev = particles->ev;

	register float xMax = worldSize.x - radius;
	register float yMax = worldSize.y - radius;

	for (register int i = 0; i < particleNumber; ++i) {
		x = i * 2;
		y = x + 1;

		vel[x] += acc[x] * deltaTime / den[i] + gravity.x * deltaTime;
		vel[y] += acc[y] * deltaTime / den[i] + gravity.y * deltaTime;

		pos[x] += vel[x] * deltaTime;
		pos[y] += vel[y] * deltaTime;

		// bound collision
		// x axis bounding check
		if (pos[x] < radius) {
			vel[x] *= wallDamping;
			pos[x] = radius;
		}
		else if (pos[x] > xMax) {
			vel[x] *= wallDamping;
			pos[x] = xMax;
		}

		// y axis bounding check
		if (pos[y] < radius) {
			vel[y] *= wallDamping;
			pos[y] = radius;
		}
		else if (pos[y] > yMax) {
			vel[y] *= wallDamping;
			pos[y] = yMax;
		}

		ev[x] = (ev[x] + vel[x]) / 2.0f;
		ev[y] = (ev[y] + vel[y]) / 2.0f;
	}
}
