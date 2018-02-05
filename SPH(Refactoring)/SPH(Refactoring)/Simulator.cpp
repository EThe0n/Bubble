#include "Simulator.hpp"
#include "Predefined.h"

#include <exception>

Simulator::Simulator()
{
	try {
		grid = new Grid(GRID_X_COUNT, GRID_Y_COUNT, CELL_SIZE);
		particles = new Particle(PARTICLE_NUMBER, PARTICLE_RADIUS);
		particles->initRandom(GRID_X_COUNT * CELL_SIZE, GRID_Y_COUNT * CELL_SIZE);
	}
	catch (std::exception e) {
		throw std::exception(e.what());
	}
}

Simulator::~Simulator()
{
	delete grid;
}

void Simulator::render(GLuint channel, GLuint particleSize)
{
	glUniform1i(channel, GRID_CHANNEL);
	grid->render();

	glUniform1i(channel, PARTICLE_CHANNEL);
	particles->render(particleSize);
}

void Simulator::update()
{
	particles->update();
}

void Simulator::simulate(float deltaTime)
{
	particles->simulate(deltaTime);
	particles->boundCollision(CELL_SIZE * GRID_X_COUNT, CELL_SIZE * GRID_Y_COUNT, RESTITUTION);
}
