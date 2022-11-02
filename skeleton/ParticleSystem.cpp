#include "ParticleSystem.h"
#include "Firework.h"
#include <memory>
void ParticleSystem::update(double t)
{
	for (auto g : _particle_generators) {
		if (g != NULL) {
			auto part = g->generateParticles();

			for (auto p : part) _particles.push_back(p);
		}
	}

	for (auto i = _particles.begin(); i != _particles.end();) {
		(*i)->integrate(t);

		(*i)->restLifeTime(t);

		if ((*i)->getLifeTime() < 0) {
			delete (*i);
			(i) = _particles.erase(i);
		}
		else ++i;
	}
}

ParticleGenerator* ParticleSystem::getParticleGenerator(std::string name)
{
	for (auto g : _particle_generators) 
		if (g != NULL && g->getName() == name) return g;
}

void ParticleSystem::generateFireworkSystem()
{
	Particle* i = new Particle({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 2);

	std::shared_ptr<SphereParticleGenerator> p;
	p.reset(new SphereParticleGenerator(20, { 0,30,0 }, i, 20));
	Firework* f = new Firework(pos, { 0,10,0 }, { 0,0,0 }, 0.99, 3, { 1,0,0,1 }, CreateShape(physx::PxSphereGeometry(1)), { p });
	_particles.push_back(f);

}
