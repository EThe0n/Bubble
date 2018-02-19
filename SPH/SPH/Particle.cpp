#include "Particle.h"

GLfloat Particle::vertexBufferData[8] = {
	-0.5f, -0.5f,
	0.5f, -0.5f,
	-0.5f,  0.5f,
	0.5f,  0.5f
};

Particle::Particle(int n)
{
	register int n2 = n * 2;
	register int bytes = sizeof(float) * n;
	register int bytes_2 = bytes * 2;

	position = new float[n2];
	velocity = new float[n2];
	accel = new float[n2];
	ev = new float[n2];
	density = new float[n];
	pressure = new float[n];
	surfaceNormal = new float[n];
	gradientColor = new float[n2];
	laplaceColor = new float[n];

	memset(position, 0, bytes_2);
	memset(velocity, 0, bytes_2);
	memset(accel, 0, bytes_2);
	memset(ev, 0, bytes_2);
	memset(density, 0, bytes);
	memset(pressure, 0, bytes);
	memset(surfaceNormal, 0, bytes);
	memset(gradientColor, 0, bytes_2);
	memset(laplaceColor, 0, bytes);

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
	glBufferData(GL_ARRAY_BUFFER, n2 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
}

Particle::~Particle()
{
	delete[] position;
	delete[] velocity;
	delete[] accel;
	delete[] ev;
	delete[] density;
	delete[] pressure;
	delete[] surfaceNormal;
	delete[] gradientColor;
	delete[] laplaceColor;

	glDeleteBuffers(1, &particlePositionBuffer);
	glDeleteBuffers(1, &billboardVertexBuffer);
	glDeleteVertexArrays(1, &vertexArrayID);
}

void Particle::initSorted(const float2& worldMax, float radius, int n)
{
	register int idx;
	register float x = radius;
	register float y = radius;
	const float size = 2.5f * radius;

	for (register int i = 0; i < n; ++i) {
		idx = i * 2;
		position[idx] = x;
		position[idx + 1] = y;

		y += size;
		if (y >= worldMax.y * 0.5f) {
			y = radius;
			x += size;
		}
	}
}

void Particle::initRandom(const float2& worldMax, float radius, int n)
{
	register int idx = 0;
	std::random_device rd;
	std::mt19937 e2(rd());
	std::uniform_real_distribution<float> xValue(radius, worldMax.x - radius);
	std::uniform_real_distribution<float> yValue(radius, worldMax.y - radius);

	for (register int i = 0; i < n; ++i) {
		idx = i * 2;
		position[idx] = xValue(e2);
		position[idx + 1] = yValue(e2);
	}
}

void Particle::update(int particleNumber)
{
	register int arraySize = particleNumber * 2;

	glBindBuffer(GL_ARRAY_BUFFER, particlePositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, arraySize * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, arraySize * sizeof(GLfloat), position);
}

void Particle::render(int particleNumber)
{
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
