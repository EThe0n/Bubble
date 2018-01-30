#include "Particle.hpp"

#include <exception>
#include <memory>

Particle::Particle(int _ParticleNumber, float _Radius)
{
	if (particleNumber <= 0) {
		throw std::exception("invalid number of particles");
	}
	if (_Radius <= 0.0f) {
		throw std::exception("invalid radius of particle");
	}

	particleNumber = _ParticleNumber;
	radius = _Radius;
	size = 2.5f * radius;

	int arraySize = particleNumber * 2;
	position	= new float[arraySize];
	speed		= new float[arraySize];
	pressure	= new float[arraySize];
	viscosity	= new float[arraySize];
	surface		= new float[arraySize];
	massDensity = new float[particleNumber];

	int bytes = sizeof(float) * particleNumber;
	int bytes_2 = bytes * 2;

	memset(position, 0, bytes_2);
	memset(speed, 0, bytes_2);
	memset(pressure, 0, bytes_2);
	memset(viscosity, 0, bytes_2);
	memset(surface, 0, bytes_2);
	memset(massDensity, 0, bytes);
}

Particle::~Particle()
{
	delete position;
	delete speed;
	delete pressure;
	delete viscosity;
	delete surface;
	delete massDensity;
}
