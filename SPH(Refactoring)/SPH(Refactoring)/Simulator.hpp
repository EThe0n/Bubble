#pragma once

#include <gl/glew.h>
#pragma comment(lib, "glew32.lib")

#include "Particle.hpp"

class Simulator
{
public :
	Simulator();
	~Simulator();

private :
	Particle* particles;

	static GLfloat	vertexBufferData[8];
	GLfloat*		dev_particlePositionSizeData;
	GLuint			vertexArrayID;
	GLuint			billboardVertexBuffer;
	GLuint			particlePositionBuffer;
};

GLfloat Simulator::vertexBufferData[8] = {
	-0.5f, -0.5f,
	0.5f, -0.5f,
	-0.5f,  0.5f,
	0.5f,  0.5f
};