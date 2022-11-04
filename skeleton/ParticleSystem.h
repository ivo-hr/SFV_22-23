#pragma once
#include "Particle.h"
#include "GaussianPartGen.h"
#include "UniformPartGen.h"
#include "Firework.h"
#include <memory>
class ParticleSystem {
protected:
	std::list<Particle*> _particles;
	std::list<ParticleGenerator*> _generators;
	Vector3 pos;
public:
	ParticleSystem(Vector3 Pos) {
		pos = Pos;
	};
	void update(double t) {
		for (auto g : _generators)
		{
			for (auto p : g->generateParticles()) {
				_particles.push_back(p);
			}
		}

		for (auto it = _particles.begin(); it != _particles.end();) {
			(*it)->integrate(t);

			if (!(*it)->isAlive()) {
				Firework* f = dynamic_cast<Firework*>(*it);
				if (f != nullptr) {
					for (auto i : f->explode()) {
						_particles.push_back(i);
					}
				}
				delete (*it);
				it = _particles.erase(it);
			}
			else {
				it++;
			}
		}

	};
	ParticleGenerator* getParticleGenerator(std::string name) {
		for (auto g : _generators) {
			if (g->getName() == name)
				return g;
		}
	};
	void generateNFireworksSystem() {
		//if (getParticleGenerator("Uniform") != nullptr) return;

		Particle* i = new Particle({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 2.0f, 2, { 0,1,0,1 }, CreateShape(physx::PxSphereGeometry(1)), false);

		std::shared_ptr<UniformParticleGenerator> p;
		p.reset(new UniformParticleGenerator("Uniform", i, 0.75, { 10,10,10 }, { 1,1,1 }, 1000));
		Firework* f = new Firework(pos, { 0,20,0 }, { 0,0,0 }, 0.99, 3.0f, 1.5, { 1,0,0,1 }, CreateShape(physx::PxSphereGeometry(3)), { p });
		_particles.push_back(f);


	};
	void generateGFireworksSystem() {
		//if (getParticleGenerator("Gauss") != nullptr) return;

		Particle* i = new Particle({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 2.0f, 2, { 0,1,0,1 }, CreateShape(physx::PxSphereGeometry(1)), false);

		std::shared_ptr<GaussianParticleGenerator> p;
		p.reset(new GaussianParticleGenerator("Gauss", i, 0.75, { 10,10,10 }, { 1,1,1 }, 1000));
		Firework* f = new Firework(pos, { 0,20,0 }, { 0,0,0 }, 0.99, 3.0f, 1.5, { 1,0,0,1 }, CreateShape(physx::PxSphereGeometry(3)), { p });
		_particles.push_back(f);

	};
	virtual ~ParticleSystem() {
		for (auto it = _particles.begin(); it != _particles.end(); it = _particles.erase(it)) {
			delete (*it);
		};
		for (auto gen = _generators.begin(); gen != _generators.end(); gen = _generators.erase(gen)) {
			delete (*gen);
		}

	};
};