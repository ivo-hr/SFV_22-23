#include "Firework.h"

Firework::Firework(Vector3 Pos, Vector3 Vel, Vector3 Acc, float damp, float mass, float time, Vector4 color, physx::PxShape* sh, std::list<std::shared_ptr<ParticleGenerator>> gens) :
	Particle(Pos, Vel, Acc, damp, mass, time, color, sh), _gens(gens)
{
}

Firework::~Firework()
{}

std::list<Particle*> Firework::explode()
{
	std::list<Particle*> l;
	for (auto g : _gens) {
		for (auto p : g->generateParticles()) {
			l.push_back(p);
		}
	}
	return l;
}
