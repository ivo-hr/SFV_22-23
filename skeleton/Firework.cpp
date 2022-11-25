#include "Firework.h"

Firework::Firework(Vector3 Pos, Vector3 Vel, Vector3 Acc, float damp, float mass, float time, Vector4 color, physx::PxShape* sh, std::list<std::shared_ptr<ParticleGenerator>> gens) :
	Particle(Pos, Vel, Acc, damp, mass, time, color, sh), _gens(gens)
{
}

Firework::~Firework()
{}

std::list<Particle*> Firework::explode()
{
	std::list<Particle*> pr;
	for (auto g : _gens)
		for (auto p : g->generateParticles()) {
			p->setVel(p->getVel() + vel/10);
			p->setPos(pose.p);
			pr.push_back(p);
		}

	return pr;
}