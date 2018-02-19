#pragma once

#define INF	1E-12f

#define WINDOW_WIDTH	1024
#define WINDOW_HEIGHT	768

// Force option
#define GRAVITY			-1.8f
#define WALL_DAMPING	-0.5f

// Grid Option
#define GRID_X_COUNT	40
#define GRID_Y_COUNT	20
#define CELL_SIZE		0.0457f

// Particle Option
#define PARTICLE_NUMBER			3000
#define PARTICLE_RADIUS			0.00608f
#define PARTICLE_REST_DENSITY	998.29f
#define PARTICLE_MASS			0.02f 
#define RESTITUTION				0.0f
#define GAS_STIFFNESS			3.0f
#define THRESHOLD				7.065f
#define SURFACE_TENSION			0.0728f
#define VISCOSITY				3.5f

// MODE
//#define _DEBUG
//#define NDEBUG

// CHANNEL - VERTEX, FRAGMENT
#define GRID_CHANNEL		1
#define PARTICLE_CHANNEL	2

#define PI 3.141f
#define M_PI 1.57f

struct float2
{
	float x;
	float y;
};

struct int2
{
	int x;
	int y;
};