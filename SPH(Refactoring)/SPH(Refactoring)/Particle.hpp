#pragma once

#include <GL/glew.h>
#pragma comment(lib, "glew32.lib")

class Particle
{
public:
	Particle(int _ParticleNumber, float _Radius);
	~Particle();

	void render(GLuint particleSize);
	void update();
	void initRandom(float xMax, float yMax);
	void initSorted(float xMax, float yMax);

public : 
	float*	position;
	float*	speed;
	float*	pressure;
	float*	viscosity;
	float*	surface;
	float*	massDensity;
	int		particleNumber;		// 파티클 개수
	float	radius;				// 파티클 하나의 반지름
	float	size;				// 파티클 하나의 사이즈

	static GLfloat	vertexBufferData[8];
	GLfloat*		dev_particlePositionSizeData;
	GLuint			vertexArrayID;
	GLuint			billboardVertexBuffer;
	GLuint			particlePositionBuffer;
	GLuint			uniformParticleSize;
private :

};