#pragma once

#include <gl/glew.h>
#pragma comment(lib, "glew32.lib")

#include "Particle.hpp"
#include "Grid.h"

class Simulator
{
public :
	Simulator();
	~Simulator();

	void render(GLuint channel, GLuint particleSize);
	void update();
	void simulate(float deltaTime);

private :
	Particle*	particles;
	Grid*		grid;
};

