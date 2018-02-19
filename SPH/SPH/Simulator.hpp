#pragma once

#include <GL/glew.h>
#pragma comment(lib, "glew32.lib")

#include "Predefined.h"
#include "Particle.h"
#include "LinkedCell.h"
#include "Grid.h"

class Simulator
{
public :
	Simulator();
	~Simulator();

	void update();
	void simulate(float deltaTime);
	void render(GLuint channel, GLuint particleSize);

private :
	void computeDensityAndPressure();
	void computeParticleDensity(float* den, float* pos, int lhs, int rhs);
	void computeForces();
	void computeParticleForces(int lhs, int rhs);
	void advection(float deltaTime);

private :
#pragma region Attribute
	int		particleNumber;
	float	radius;
	float	particleSize;

	float	kernel;
	float	particleMass;
	
	float2	worldSize;
	float	cellSize;
	int2	cellNumber;
	int		cellTotalNumber;

	float2	gravity;
	float	wallDamping;
	float	restDensity;
	float	gasStiffness;
	float	viscosity;
	float	surfaceNormal;

	float	surfaceCoef;
	float	poly6Coef;
	float	spikyCoef;
	float	viscosityCoef;
	float	gradPoly6;
	float	laplacePoly6;
	
	float	kernel_2;
	float	selfDensity;
	float	selfLaplaceColor;

#pragma endregion
	
	Particle*	particles;
	LinkedCell* linkedCell;
	Grid*		grid;
};