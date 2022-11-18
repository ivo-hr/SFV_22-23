#pragma once
#include "Particle.h"
#include "GaussianPartGen.h"
#include "UniformPartGen.h"
#include "Firework.h"
#include "ForceGenerator.h"
#include "ParticleForceReg.h"
#include <memory>
#include <random>
class ParticleSystem {

public:
	ParticleSystem(Vector3 Pos) {
		pos = Pos;
	};
	void update(double t) {
		for (auto g : _generators)
			for (auto p : g->generateParticles()) {
				_particles.push_back(p);
				_registry.addReg(activeForce, p);	//no furula :(
			}


		_registry.updateForces(t);
		
		for (auto it = _particles.begin(); it != _particles.end();) {
			(*it)->integrate(t);

			if (!(*it)->isAlive()) {
				Firework* f = dynamic_cast<Firework*>(*it);
				if (f != nullptr)
					for (auto i : f->explode())
						_particles.push_back(i);
				_registry.deletePartReg(*it);
				delete (*it);
				it = _particles.erase(it);
			}
			else it++;
		}
	};
	ParticleGenerator* getParticleGenerator(std::string name) {
		for (auto g : _generators)
			if (g->getName() == name)
				return g;
		return nullptr;
	};
	void generateNFireworksSystem() {
		if (getParticleGenerator("Uniform") != nullptr) return;

		Particle* i = new Particle({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 2.0f, 5, { 0,1,1,1 }, CreateShape(physx::PxSphereGeometry(1)), false);
		_registry.addReg(activeForce, i);
		std::shared_ptr<UniformParticleGenerator> p;
		p.reset(new UniformParticleGenerator("Uniform", i, 0.75, { 10,10,10 }, { 15,15,15 }, 1000));
		Firework* f = new Firework(pos, { 0,10,0 }, { 0,90,0 }, 0.99, 3.0f, 2, { 1,0,0,1 }, CreateShape(physx::PxSphereGeometry(3)), { p });
		_particles.push_back(f);
		_registry.addReg(activeForce, f);
		
	};
	void generateGFireworksSystem() {
		if (getParticleGenerator("Gauss") != nullptr) return;

		Particle* i = new Particle({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 2.0f, 5, { 1,0,1,1 }, CreateShape(physx::PxSphereGeometry(1)), false);
		_registry.addReg(activeForce, i);
		std::shared_ptr<GaussianParticleGenerator> p;
		p.reset(new GaussianParticleGenerator("Gauss", i, 0.75, { 10,10,10 }, { 10,10,10 }, 1000));
		Firework* f = new Firework(pos, { 0,20,0 }, { 0,50,0 }, 0.99, 3.0f, 2, { 0,0,1,1 }, CreateShape(physx::PxSphereGeometry(3)), { p });
		_particles.push_back(f);
		_registry.addReg(activeForce, f);
	};
	void generateWhateverSystem() {
		if (getParticleGenerator("Whatever") != nullptr) return;

		Particle* i = new Particle({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 200.0f, 2, { 0,0,0,1 }, CreateShape(physx::PxSphereGeometry(1)), false);
		_registry.addReg(activeForce, i);
		std::shared_ptr<GaussianParticleGenerator> p;
		p.reset(new GaussianParticleGenerator("Gauss", i, 0.5, { 10,10,10 }, { 10,10,10 }, 1000));
		Firework* f = new Firework(pos, { 0,0,0 }, { 0,0,0 }, 0.5, 1.0f, 0, { 1,1,1,1 }, CreateShape(physx::PxBoxGeometry(Vector3(1,1,1))), { p });
		_particles.push_back(f);
		_registry.addReg(activeForce, f);
	};

	
	virtual ~ParticleSystem() {
		for (auto it = _particles.begin(); it != _particles.end(); it = _particles.erase(it)) {
			delete (*it);
		};
		for (auto gen = _generators.begin(); gen != _generators.end(); gen = _generators.erase(gen)) {
			delete (*gen);
		}

	};


	void gravGen(int num) {
		
		GravForceGen* gen = new GravForceGen(Vector3(0, -9.8, 0), 100);
		
		for (int i = 0; i < num; i++) {
			Particle* part = new Particle({ 10,(float)i,10 }, { 0,0,0 }, { 0,0,0 },
				0.99, 10, 100000, { 0,0,1,1 }, CreateShape(physx::PxSphereGeometry(1)), true);
			_registry.addReg(gen, part);
			_particles.push_back(part);
		}

		activeForce = 1;
	}
	void WindGen(int num) {
		
		WindForceGen* gen = new WindForceGen(Vector3(5, 0, 0), 1, 0.1);

		for (int i = 0; i < num; i++) {
			Particle* part = new Particle({ 10,(float)i,10 }, { 0,0,0 }, { 0,0,0 },
				0.99, 10, 100000, { 0,1,0,1 }, CreateShape(physx::PxSphereGeometry(1)), true);
			_registry.addReg(gen, part);
			_particles.push_back(part);
		}
		activeForce = 2;
	}
	void TornadoGen(int num) {

		
		TornadoForceGen* gen = new TornadoForceGen(Vector3(0, 0, 0), 1, 0.1, 0.75);
		
		
		for (int i = 0; i < num; i++) {
			Particle* part = new Particle({ 10,(float)i,10}, { 0,0,0 }, { 0,0,0 },
				0.99, 10, 100000, { 0,0,0,1 }, CreateShape(physx::PxSphereGeometry(1)), true);
			_registry.addReg(gen, part);
			_particles.push_back(part);
		}
		
		activeForce = 3;
		
	}


protected:
	std::list<Particle*> _particles;
	std::list<ParticleGenerator*> _generators;
	
	ParticleForceReg _registry;
	Vector3 pos;
	int activeForce = 1;
};