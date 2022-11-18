#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include <list>
#include <string>
#include "Particle.h"
#include "ParticleForceReg.h"

class ParticleGenerator {
public:
	virtual std::list<Particle*> generateParticles() = 0;
	std::string getName() const { return _name; }
	void setName(std::string name) { _name = name; }
	void setParticle(Particle* part) { _model = part; }
	virtual ~ParticleGenerator() {};


	Vector3 _mean_pos, _mean_vel, _mean_acc;
	Vector4 _modelColor;
	double _generation_probability;
	Particle* _model;
	int _num_particles;
	std::string _name;
	physx::PxShape* _shape;
	bool active = true;
};