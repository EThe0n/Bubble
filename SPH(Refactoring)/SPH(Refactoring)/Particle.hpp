#pragma once

#include <GL/glew.h>
#pragma comment(lib, "glew32.lib")

#include "LinkedCell.h"

class Particle
{
public:
	Particle(int _ParticleNumber, float _Radius, LinkedCell* cell);
	~Particle();

	void render(GLuint particleSize);
	void update();
	void simulate(float deltaTime);
	void boundCollision(float xMax, float yMax, float restitution);
	void initRandom(float xMax, float yMax);
	void initSorted(float xMax, float yMax);
	
private : 
	float*		position;
	float*		speed;
	float*		pressure;
	float*		viscosity;
	float*		surface;
	float*		massDensity;
	int			particleNumber;		// ��ƼŬ ����
	float		radius;				// ��ƼŬ �ϳ��� ������
	float		size;				// ��ƼŬ �ϳ��� ������
	LinkedCell*	linkedCell;
	
	static GLfloat	vertexBufferData[8];
	GLfloat*		dev_particlePositionSizeData;
	GLuint			vertexArrayID;
	GLuint			billboardVertexBuffer;
	GLuint			particlePositionBuffer;
	GLuint			uniformParticleSize;

private :
	
};