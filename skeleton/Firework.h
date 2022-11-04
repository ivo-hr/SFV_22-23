#pragma once
#include "Particle.h"
#include "ParticleGenerator.h"
#include <list>
#include <memory>

class Firework : public Particle
{
public:
	Firework(Vector3 Pos, Vector3 Vel, Vector3 Acc, float damp, float mass, float time, Vector4 color = { 1,1,1,1 }, physx::PxShape* sh = CreateShape(physx::PxSphereGeometry(1)), std::list<std::shared_ptr<ParticleGenerator>> gens = {});
	virtual ~Firework();
	std::list<Particle*> explode();
protected:
	std::list<std::shared_ptr<ParticleGenerator>> _gens;
};

