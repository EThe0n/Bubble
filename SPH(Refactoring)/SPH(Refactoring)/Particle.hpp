#pragma once

class Particle
{
public:
	Particle(int _ParticleNumber, float _Radius);
	~Particle();

public : 
	float*	position;
	float*	speed;
	float*	pressure;
	float*	viscosity;
	float*	surface;
	float*	massDensity;
	int		particleNumber;
	float	radius;
	float	size;
};