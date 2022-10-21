#include "ParticleSystem.h"

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

}
