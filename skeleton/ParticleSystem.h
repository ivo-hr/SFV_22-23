#pragma once
#include <list>
#include <vector>
#include "Particle.h"

#include "ParticleGenerator.h"
#include "NormalPartGen.h"
#include "GaussianPartGen.h"

class ParticleSystem
{
public:
	ParticleSystem(Vector3 p) { pos = p; };
	void update(double t);
	ParticleGenerator* getParticleGenerator(std::string name);
	void generateFireworkSystem();

protected:
	std::list<Particle*> _particles;
	std::list<ParticleGenerator*> _particle_generators;
	Vector3 pos;

	
};

