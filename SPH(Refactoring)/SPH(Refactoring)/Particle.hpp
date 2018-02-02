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
	int		particleNumber;		// ��ƼŬ ����
	float	radius;				// ��ƼŬ �ϳ��� ������
	float	size;				// ��ƼŬ �ϳ��� ������

	static GLfloat	vertexBufferData[8];
	GLfloat*		dev_particlePositionSizeData;
	GLuint			vertexArrayID;
	GLuint			billboardVertexBuffer;
	GLuint			particlePositionBuffer;
	GLuint			uniformParticleSize;
private :

};