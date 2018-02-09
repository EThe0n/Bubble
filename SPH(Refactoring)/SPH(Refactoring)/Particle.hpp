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
	void calcMassDensity(int cellNumber, float h, float mass);
	void calcPressureField(float gasStiffness, float restDensity);
	void calcForces(int cellNumber, float surfaceTension, float vis, float h, float mass);
	void collisionHandling(int cellNumber);
	void initRandom(float xMax, float yMax);
	void initSorted(float xMax, float yMax);
	
private : 
	float*		position;
	float*		speed;
	float*		pressure;
	float*		pressureField;
	float*		viscosity;
	float*		surface;
	float*		massDensity;
	int			particleNumber;		// 파티클 개수
	float		radius;				// 파티클 하나의 반지름
	float		size;				// 파티클 하나의 사이즈
	LinkedCell*	linkedCell;
	
	static GLfloat	vertexBufferData[8];
	GLfloat*		dev_particlePositionSizeData;
	GLuint			vertexArrayID;
	GLuint			billboardVertexBuffer;
	GLuint			particlePositionBuffer;
	GLuint			uniformParticleSize;

private :
	void particleCollision(int lhsIndex, int rhsIndex);
	void particleMassDensity(int lhsIndex, int rhsIndex, float coef, float h);
	void particlePressureForce(int lhsIndex, int rhsIndex, float coef, float h);
	void particleViscosityForce(int lhsIndex, int rhsIndex, float coef, float h);
};