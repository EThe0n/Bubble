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

	void render(GLuint channel);
	void update();

private :
	Particle*	particles;
	Grid*		grid;
};

