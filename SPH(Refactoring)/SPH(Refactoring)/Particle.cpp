#include "Particle.hpp"

#include <exception>
#include <memory>
#include <random>

GLfloat Particle::vertexBufferData[8] = {
	-0.5f, -0.5f,
	0.5f, -0.5f,
	-0.5f,  0.5f,
	0.5f,  0.5f
};

Particle::Particle(int _ParticleNumber, float _Radius)
{
	if (particleNumber <= 0) {
		throw std::exception("invalid number of particles");
	}
	if (_Radius <= 0.0f) {
		throw std::exception("invalid radius of particle");
	}

	particleNumber = _ParticleNumber;
	radius = _Radius;
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
	dev_particlePositionSizeData = new GLfloat[arraySize];

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

	delete dev_particlePositionSizeData;
	glDeleteBuffers(1, &particlePositionBuffer);
	glDeleteBuffers(1, &billboardVertexBuffer);
	glDeleteVertexArrays(1, &vertexArrayID);
}

void Particle::render()
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
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 2, particleNumber);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Particle::update()
{
	register int arraySize = particleNumber * 2;

	glBindBuffer(GL_ARRAY_BUFFER, particlePositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, arraySize * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, arraySize * sizeof(GLfloat), dev_particlePositionSizeData);
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
