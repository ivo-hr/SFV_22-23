#pragma once
#include <list>
#include <string>
#include "core.hpp"
#include "RenderUtils.hpp"
#include "Particle.h"

class ParticleGenerator {
public:
	
	virtual std::list<Particle*> generateParticles() = 0;
	std::string getName() { return name; };
protected:
	Vector3 avgPos, avgVel, avgAcc;
	Vector4 col;
	double avgTim, genProb;
	float damp;
	int maxPart;
	std::string name;
	
};

