#include "Simulator.hpp"
#include "Predefined.h"

#include <exception>

Simulator::Simulator()
{
	try {
		grid = new Grid(GRID_X_COUNT, GRID_Y_COUNT, CELL_SIZE);
	}
	catch (std::exception e) {
		throw std::exception(e.what());
	}
}

Simulator::~Simulator()
{
	delete grid;
}

void Simulator::render(GLuint channel)
{
	glUniform1i(channel, GRID_CHANNEL);
	grid->render();

	//glUniform1i(channel, PARTICLE_CHANNEL);
	//particles->render();
}

void Simulator::update()
{
	//particles->update();
}
