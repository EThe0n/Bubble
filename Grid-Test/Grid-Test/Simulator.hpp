#pragma once

#include <list>
#include <vector>
using std::list;
using std::vector;

#include <glm/glm.hpp>
using glm::vec3;

static const GLfloat g_vertex_buffer_data[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f,
	0.5f,  0.5f, 0.0f,
};

struct Particle
{
	// attributes
	vec3 position;
	vec3 speed;
	vec3 leapFrog;
	float massDensity;

	// forces
	vec3 pressure;
	vec3 gravity;
	vec3 viscosity;
	vec3 surface;

	float size;
};

typedef list<Particle*> Cell;

class Simulator
{
public :
	Simulator();
	~Simulator();

	void render();
	void simulate(float deltaTime);
	void update();
	
private :
	Particle*		particleContainer;
	vector<Cell>	cellContainer;

	// index 0		: array size
	// index 1 ~	: neighbor cell's index
	int**			cellNeighborIndex;

	GLfloat*		dev_particlePositionSizeData;
	GLubyte*		dev_particleColorData;

	GLuint			vertexArrayID;
	GLuint			billboardVertexBuffer;
	GLuint			particlePositionBuffer;
	int				particleCount;
	int				lastUsedParticleIndex;

private :
	void			initNeighborCellIndex(int nCell);
	void			boundCollision();
	void			collisionHandling();
	void			calcMassDensity();
	float			KernelFunction(float r, float h);
	float			GradientKernelFunction(float r, float h);
	float			LaplacianKernelFunction(float r, float h);
};