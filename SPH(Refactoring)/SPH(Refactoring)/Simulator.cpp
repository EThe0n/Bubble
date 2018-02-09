#include "Simulator.hpp"
#include "Predefined.h"

#include <exception>

Simulator::Simulator()
{
	try {
		grid = new Grid(GRID_X_COUNT, GRID_Y_COUNT, CELL_SIZE);
		LinkedCell* cell = new LinkedCell(PARTICLE_NUMBER, CELL_SIZE, GRID_X_COUNT, GRID_Y_COUNT);
		particles = new Particle(PARTICLE_NUMBER, PARTICLE_RADIUS, cell);
		particles->initSorted(GRID_X_COUNT * CELL_SIZE, GRID_Y_COUNT * CELL_SIZE);
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
	try {
		particles->update();
	}
	catch (std::exception& e) {
		throw e.what();
	}
}

void Simulator::simulate(float deltaTime)
{
	const int cellNumber = GRID_X_COUNT * GRID_Y_COUNT;

	particles->calcMassDensity(cellNumber, CELL_SIZE, PARTICLE_MASS);
	particles->calcPressureField(GAS_STIFFNESS, PARTICLE_REST_DENSITY);
	particles->calcForces(cellNumber, SURFACE_TENSION, VISCOSITY, CELL_SIZE, PARTICLE_MASS);
	particles->simulate(deltaTime);
	particles->collisionHandling(cellNumber);
	particles->boundCollision(CELL_SIZE * GRID_X_COUNT, CELL_SIZE * GRID_Y_COUNT, RESTITUTION);
}
