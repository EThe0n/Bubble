#include "Particle.hpp"

#include <exception>
#include <memory>
#include <random>
#include <algorithm>

using std::max;
using std::min;

GLfloat Particle::vertexBufferData[8] = {
	-0.5f, -0.5f,
	0.5f, -0.5f,
	-0.5f,  0.5f,
	0.5f,  0.5f
};

Particle::Particle(int _ParticleNumber, float _Radius, LinkedCell* cell = nullptr) :
	particleNumber(_ParticleNumber), radius(_Radius), linkedCell(cell)
{
	if (cell == nullptr) {
		throw std::exception("invalid linked cell");
	}
	if (_ParticleNumber <= 0) {
		throw std::exception("invalid number of particles");
	}
	if (_Radius <= 0.0f) {
		throw std::exception("invalid radius of particle");
	}
	size = 2.5f * radius;
	
	register int arraySize = particleNumber * 2;
	position	= new float[arraySize];
	speed		= new float[arraySize];
	pressure	= new float[arraySize];
	viscosity	= new float[arraySize];
	surface		= new float[arraySize];
	massDensity = new float[particleNumber];

	register int bytes = sizeof(float) * particleNumber;
	register int bytes_2 = bytes * 2;

	memset(position, 0, bytes_2);
	memset(speed, 0, bytes_2);
	memset(pressure, 0, bytes_2);
	memset(viscosity, 0, bytes_2);
	memset(surface, 0, bytes_2);
	memset(massDensity, 0, bytes);
	
	// Rendering setting
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glGenBuffers(1, &billboardVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboardVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles
	glGenBuffers(1, &particlePositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, particlePositionBuffer);

	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, arraySize * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
}

Particle::~Particle()
{
	delete position;
	delete speed;
	delete pressure;
	delete viscosity;
	delete surface;
	delete massDensity;
	delete linkedCell;

	glDeleteBuffers(1, &particlePositionBuffer);
	glDeleteBuffers(1, &billboardVertexBuffer);
	glDeleteVertexArrays(1, &vertexArrayID);
}

void Particle::render(GLuint particleSize)
{
	glUniform1f(particleSize, size);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billboardVertexBuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, particlePositionBuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size : (x, y) = 2
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// These functions are specific to glDrawArrays*Instanced*.
	// The first parameter is the attribute buffer we're talking about.
	// The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1

								 // Draw the particules !
								 // This draws many times a small triangle_strip (which looks like a quad).
								 // This is equivalent to :
								 // for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4), 
								 // but faster!
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleNumber);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Particle::update()
{
	register int arraySize = particleNumber * 2;

	glBindBuffer(GL_ARRAY_BUFFER, particlePositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, arraySize * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, arraySize * sizeof(GLfloat), position);

	try {
		linkedCell->update(position);
	}
	catch (std::exception& e) {
		throw e.what();
	}
}

void Particle::simulate(float deltaTime)
{
	register int idx = 0;
	
	for (register int i = 0; i < particleNumber; ++i) {
		idx = i * 2 + 1;
		speed[idx] -= 9.81f * deltaTime;
	}

	register int x;
	register int y;
	for (register int i = 0; i < particleNumber; ++i) {
		x = i * 2;
		y = x + 1;

		position[x] += speed[x] * deltaTime;
		position[y] += speed[y] * deltaTime;
	}
}

void Particle::initRandom(float xMax, float yMax)
{
	register int idx = 0;
	std::random_device rd;
	std::mt19937 e2(rd());
	std::uniform_real_distribution<float> xValue(0.0f, xMax);
	std::uniform_real_distribution<float> yValue(0.0f, yMax);

	for (register int i = 0; i < particleNumber; ++i) {
		idx = i * 2;
		position[idx] = xValue(e2);
		position[idx + 1] = yValue(e2);
	}
}

void Particle::initSorted(float xMax, float yMax)
{

}

void Particle::particleCollision(int lhsIndex, int rhsIndex)
{
	register int lhs_x = lhsIndex * 2;
	register int lhs_y = lhs_x + 1;
	register int rhs_x = rhsIndex * 2;
	register int rhs_y = rhs_x + 1;

	float dx = position[lhs_x] - position[rhs_x];
	float dy = position[lhs_y] - position[rhs_y];
	float distance = dx * dx + dy * dy;
	float detect = distance - radius * radius;
	float normal[2];
	float d;
	float dot;
	float dv[2];
	int sig;

	if (detect <= 0.0f) {
		// get normal vector
		sig = (detect > 0) - (detect < 0);
		if (distance == 0.0f) {
			dx = -speed[rhs_x];
			dy = -speed[rhs_y];
			distance = sqrtf(dx * dx + dy * dy);
			normal[0] = dx / distance;
			normal[1] = dy / distance;
			d = 2.0f * radius;
		}
		else {
			distance = sqrtf(distance);
			normal[0] = sig * -dx / distance;
			normal[1] = sig * -dy / distance;
			d = 2.0f * radius - distance;
		}

		// move to collision point
		position[rhs_x] += normal[0] * d;
		position[rhs_y] += normal[1] * d;

		// change speed
		dot = speed[rhs_x] * normal[0] + speed[rhs_y] * normal[1];
		dv[0] = dot * normal[0];
		dv[1] = dot * normal[1];
		speed[rhs_x] -= dv[0];
		speed[rhs_y] -= dv[1];
		speed[lhs_x] += dv[0];
		speed[lhs_y] += dv[1];
	}
}

void Particle::boundCollision(float xMax, float yMax, float restitution)
{
	register int x = 0;
	register int y = 0;
	register float normal[2] = { 0.0f };
	register float length = 0.0f;
	register float coef = 0.0f;

 	xMax -= radius;
	yMax -= radius;

	for (register int i = 0; i < particleNumber; ++i) {
		memset(normal, 0, sizeof(float) * 2);
		x = i * 2;
		y = x + 1;

		// x axis bounding check
		if (position[x] < radius) {
			normal[0] += 1.0f;
			position[x] = radius;
		}
		else if (position[x] > xMax) {
			normal[0] -= 1.0f;
			position[x] = xMax;
		}

		// y axis bounding check
		if (position[y] < radius) {
			normal[1] += 1.0f;
			position[y] = radius;
		}
		else if (position[y] > yMax) {
			normal[1] -= 1.0f;
			position[y] = yMax;
		}

		// calculate force
		register float length = sqrt(normal[0] * normal[0] + normal[1] * normal[1]);
		if (length > 1.0f) {
			normal[0] /= length; 
			normal[1] /= length;
		}

		if (length >= 1.0f) {
			coef = (1.0f + restitution) * (speed[x] * normal[0] + speed[y] * normal[1]);
			speed[x] = speed[x] - coef * normal[0];
			speed[y] = speed[y] - coef * normal[1];
		}
	}
}

void Particle::collisionHandling(int cellNumber)
{
	register int neighbor[5];
	register int neighborCount;
	register int start, end;
	register int neighborStart, neighborEnd;
	register int pIdx;
	
	for (register int cellIdx = 0; cellIdx < cellNumber; ++cellIdx) {
		start = linkedCell->cellStartIndex[cellIdx];
		end = start + linkedCell->particleNumberInCell[cellIdx];

		neighborCount = linkedCell->neighborCellSize[cellIdx];
		memcpy(neighbor, linkedCell->neighborCellIndex[cellIdx], sizeof(int) * neighborCount);

		for (register int i = start; i < end; ++i) {
			pIdx = linkedCell->particleIndex[i];

			// same cell collision check
			for (register int j = i + 1; j < end; ++j) {
				particleCollision(pIdx, linkedCell->particleIndex[j]);
			}
			
			// neighbor cells collision check 
			for (register int n = 0; n < neighborCount; ++n) {
				neighborStart = linkedCell->cellStartIndex[neighbor[n]];
				neighborEnd = neighborStart + linkedCell->particleNumberInCell[neighbor[n]];
				for (register int j = neighborStart; j < neighborEnd; ++j) {
					particleCollision(pIdx, linkedCell->particleIndex[j]);
				}
			}
		}
	}
}
