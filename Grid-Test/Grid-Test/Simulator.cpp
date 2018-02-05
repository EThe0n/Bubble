#include <algorithm>
using std::sort;

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

#include "Simulator.hpp"
#include "Predefined.hpp"

#define PARTICLE_PLACE_RANDOM
//#define PARTICLE_PLACE_DAM

Simulator::Simulator()
{
	const int p_size = MAX_PARTICLE * 4;
	const int c_size = NUM_CELL_X * NUM_CELL_Y;

	cellContainer.resize(c_size);
	particleContainer = new Particle[MAX_PARTICLE];
	dev_particlePositionSizeData = new GLfloat[p_size];
	dev_particleColorData = new GLubyte[p_size];
#ifdef PARTICLE_PLACE_DAM
	float xPos = X_MIN;
	float yPos = Y_MIN;
	for (int i = 0; i < MAX_PARTICLE; i++) {
		// initialize particle
		Particle& p = particleContainer[i];
		p.position = vec3(xPos, yPos, 0);
		p.size = PARTICLE_SIZE;
		p.massDensity = REST_DENSITY;
		xPos += RADIUS * 2.5f;
		if (xPos >= X_MAX) {
			xPos = X_MIN;
			yPos += RADIUS * 3.0f;
		}
	}
#endif
#ifdef PARTICLE_PLACE_RANDOM
	for (int i = 0; i < MAX_PARTICLE; i++) {
		// initialize particle
		Particle& p = particleContainer[i];
		p.position = vec3(rand() % (X_MAX - X_MIN) + X_MIN, rand() % (Y_MAX - Y_MIN) + Y_MIN, 0);
		p.size = PARTICLE_SIZE;
		p.massDensity = REST_DENSITY;
	}
#endif
	particleCount = MAX_PARTICLE;

	// initialize linked cell
	updateLinkedCell();
	
	// initialize neighbor cell index
	initNeighborCellIndex(c_size);

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glGenBuffers(1, &billboardVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboardVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles
	glGenBuffers(1, &particlePositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, particlePositionBuffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, p_size * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
}

Simulator::~Simulator()
{
	const int c_size = ((X_MAX - X_MIN) / CELL_SIZE) * ((Y_MAX - Y_MIN) / CELL_SIZE);
	for (register int i = 0; i < c_size; ++i) {
		delete[] cellNeighborIndex[i];
	}
	delete[] cellNeighborIndex;
	delete[] dev_particleColorData;
	delete[] dev_particlePositionSizeData;
	delete[] particleContainer;

	glDeleteBuffers(1, &particlePositionBuffer);
	glDeleteBuffers(1, &billboardVertexBuffer);
	glDeleteVertexArrays(1, &vertexArrayID);
}

void Simulator::render()
{
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billboardVertexBuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
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
		4,                                // size : x + y + z + size => 4
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
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleCount);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Simulator::simulate(float deltaTime)
{
	calcMassDensity();

	for (int i = 0; i < MAX_PARTICLE; i++) {
		Particle& p = particleContainer[i];
		vec3 totalForce = p.gravity + p.pressure + p.viscosity + p.surface;
		vec3 accel = totalForce / p.massDensity;

		p.speed += accel * deltaTime * 100.0f;
	}

	for (int i = 0; i < MAX_PARTICLE; i++) {
		Particle& p = particleContainer[i]; // shortcut
		p.position += p.speed * (float)deltaTime * 100.0f; // meter to centimeter
	}

	//// object collision handling
	//for (int i = 0; i < NUM_CELL; i++) {
	//	collisionHandling(i);
	//}

	// boundary collsiion handling
	for (int i = 0; i < MAX_PARTICLE; i++) {
		Particle& p = particleContainer[i];
		boundCollision(p);
	}

	updateLinkedCell();

	particleCount = 0;
	for (register int i = 0; i < MAX_PARTICLE; i++) {
		Particle& p = particleContainer[i]; // shortcut

		// Fill the GPU buffer
		int size = 4 * particleCount;
		dev_particlePositionSizeData[size + 0] = p.position.x;
		dev_particlePositionSizeData[size + 1] = p.position.y;
		dev_particlePositionSizeData[size + 2] = 0.0f;
		dev_particlePositionSizeData[size + 3] = p.size;

		particleCount++;
	}
}

void Simulator::update()
{
	int size = MAX_PARTICLE * 4;
	glBindBuffer(GL_ARRAY_BUFFER, particlePositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * sizeof(GLfloat) * 4, dev_particlePositionSizeData);
}

int Simulator::getCellIndex(const vec3 & position)
{
	register int x = (position.x - X_MIN) / CELL_SIZE;
	register int y = (position.y - Y_MIN) / CELL_SIZE;
	return y * NUM_CELL_X + x;
}

void Simulator::updateLinkedCell()
{
	for (auto& cell : cellContainer) {
		cell.clear();
	}

	for (int i = 0; i < MAX_PARTICLE; i++) {
		cellContainer[getCellIndex(particleContainer[i].position)].push_back(&particleContainer[i]);
	}
}

void Simulator::initNeighborCellIndex(int nCell)
{
	const int offsetX[] = { 1, -1, 0, 1 };
	const int offsetY[] = { 0, 1, 1, 1 };
	register int index[4];
	register int indexCount = 0;

	cellNeighborIndex = new int*[nCell];
	
	for (register int y = 0; y < NUM_CELL_Y; ++y) {
		for (register int x = 0; x < NUM_CELL_X; ++x) {
			register int idx = y * NUM_CELL_Y + x;

			indexCount = 0;
			for (register int k = 0; k < 4; ++k) {
				int _x = x + offsetX[k];
				int _y = y + offsetY[k];
				if (_x >= 0 && _x < NUM_CELL_X && _y >= 0 && _y < NUM_CELL_Y) {
					index[indexCount] = _y * NUM_CELL_Y + _x;
					++indexCount;
				}
			}
			
			cellNeighborIndex[idx] = new int[indexCount + 1];
			cellNeighborIndex[idx][0] = indexCount;
			memcpy(&cellNeighborIndex[idx][1], index, sizeof(int) * indexCount);
		}
	}
}

void Simulator::boundCollision(Particle& p)
{
	vec3 normal = vec3(0.0f, 0.0f, 0.0f);

	if (p.position.x < X_MIN + RADIUS) {
		normal += vec3(1, 0, 0);
		p.position.x = X_MIN + RADIUS;
	}
	if (p.position.y < Y_MIN + RADIUS) {
		normal += vec3(0, 1, 0);
		p.position.y = Y_MIN + RADIUS;
	}
	if (p.position.x > X_MAX - RADIUS) {
		normal += vec3(-1, 0, 0);
		p.position.x = X_MAX - RADIUS;
	}
	if (p.position.y > Y_MAX - RADIUS) {
		normal += vec3(0, -1, 0);
		p.position.y = Y_MAX - RADIUS;
	}
	
	if (glm::length(normal) > 1.0f) {
		normal = normalize(normal);
	}

	float alpha = glm::length(p.speed);
	
	if (alpha > 0.0f) {
		vec3 speed = glm::normalize(p.speed);
		speed = (1 + RESTITUTION) * (glm::dot(-speed, normal)) * normal + speed;
		p.speed = speed * alpha;
	}
}

void Simulator::collisionHandling(int cellIndex)
{
	int neighbor[5] = { 0 };
	int neighborCount = cellNeighborIndex[cellIndex][0];
	Cell& c = cellContainer[cellIndex];

	memcpy(neighbor, cellNeighborIndex[cellIndex], sizeof(int) * (neighborCount + 1));
	
	register int size = c.size();
	for (register int i = 0; i < size; i++) {
		Particle& lhs = *c[i];

		for (register int j = i + 1; j < size; j++) {
			Particle& rhs = *c[j];

			float d = glm::length(lhs.position - rhs.position);

			if (d <= 0.0f) {
				float speed = rhs.speed.x * rhs.speed.x + rhs.speed.y * rhs.speed.y;
				vec3 n;
				if (speed >= 0) {
					n = vec3(0, 1, 0);
				}
				else {
					n = -glm::normalize(rhs.speed);
				}
				rhs.position += RADIUS * n;
				rhs.speed = rhs.speed - glm::dot(rhs.speed, n) * n;
			}
			if (d > 0.0f && d < 2.0f * RADIUS) {
				vec3 n = glm::normalize(rhs.position - lhs.position);

				// move rhs particle's position
				rhs.position += n * (2.0f * RADIUS - d);

				// change rhs speed
				rhs.speed = rhs.speed - glm::dot(rhs.speed, n) * n;
			}
		}

		for (register int j = 1; j <= neighborCount; j++) {
			Cell& neighborCell = cellContainer[neighbor[j]];

			for (Particle* rhs : neighborCell) {
				float d = glm::length(lhs.position - rhs->position);

				if (d > 0.0f && d < 2.0f * RADIUS) {
					vec3 n = glm::normalize(rhs->position - lhs.position);

					// move rhs particle's position
					rhs->position += n * (2.0f * RADIUS - d);

					// change rhs speed
					rhs->speed -= glm::dot(rhs->speed, n) * n;
				}
			}
		}
	}
}

void Simulator::calcMassDensity()
{


}
