#pragma once

#include <GL/glew.h>
#pragma comment(lib, "glew32.lib")

#include <memory>
#include <random>
#include "Predefined.h"

class Particle
{
public :
	Particle(int n);
	~Particle();

	void initSorted(const float2& worldMax, float radius, int n);
	void initRandom(const float2& worldMax, float radius, int n);
	void update(int particleNumber);
	void render(int particleNumber);
	
public :
	float* position;
	float* velocity;
	float* accel;
	float* ev;
	float* density;
	float* pressure;
	float* surfaceNormal;
	float* gradientColor;
	float* laplaceColor;
private :
	static GLfloat	vertexBufferData[8];
	GLfloat*		dev_particlePositionSizeData;
	GLuint			vertexArrayID;
	GLuint			billboardVertexBuffer;
	GLuint			particlePositionBuffer;
	GLuint			uniformParticleSize;
};