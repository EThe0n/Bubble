#pragma once

// Window
#define WIDTH		1024
#define HEIGHT		768

// Camera
#define RATIO_4_3	1.333f
#define NEAR		0.1f
#define FAR			10000.0f

// For texture
#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

// Coordinate
#define X_MIN		0 
#define X_MAX		100

#define Y_MIN		0
#define Y_MAX		100

#define Z_MIN		0
#define Z_MAX		10

// Cell & Particle
#define CELL_SIZE	5
#define NUM_CELL_X	((X_MAX - X_MIN) / CELL_SIZE)
#define NUM_CELL_Y	((Y_MAX - Y_MIN) / CELL_SIZE)
#define NUM_CELL	NUM_CELL_X * NUM_CELL_Y

#define PARTICLE_SIZE 2
#define MAX_PARTICLE 10000

// Rendering Option
#define GRID_CHANNEL 1
#define PARTICLE_CHANNEL 2

// Constant
#define PI			3.14f
#define M_PI		1.57f

#define REST_DENSITY	998.29f
#define GAS_STIFFNESS	3