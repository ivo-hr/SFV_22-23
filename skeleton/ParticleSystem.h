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
		if (_particles.size() < 1000) {
			for (auto g : _generators) {
				for (auto p : g->generateParticles()) {
					_particles.push_back(p);
					_registry.addReg(activeForce, p);	//no furula :(
				}
			}
		}


		_registry.updateForces(t);
		
		for (auto it = _particles.begin(); it != _particles.end();) {
			(*it)->integrate(t);

			if (!(*it)->isAlive()) {
				Firework* f = dynamic_cast<Firework*>(*it);
				if (f != nullptr)
					for (auto i : f->explode()) {
						if (_particles.size() < 1000) {
							_particles.push_back(i);
							_registry.addReg(activeForce, i);
						}
					}
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
	void generateNFireworksSystem(int scale) {
		if (getParticleGenerator("Uniform") != nullptr) return;

		Particle* i = new Particle({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 2.0f, 10, { 1,1,0,1 }, CreateShape(physx::PxSphereGeometry(scale)), false);
		_registry.addReg(activeForce, i);
		std::shared_ptr<UniformParticleGenerator> p;
		p.reset(new UniformParticleGenerator("Uniform", i, 0.75, { 10,10,10 }, { 200,200,200 }, 100));
		Firework* f = new Firework(pos, { 0,10,0 }, { 0,(float)scale,0 }, 0.99, 3.0f, 2, { 1,0,1,1 }, CreateShape(physx::PxSphereGeometry(scale)), { p });
		_particles.push_back(f);
		_registry.addReg(activeForce, f);
		
	};
	void generateGFireworksSystem(int scale) {
		if (getParticleGenerator("Gauss") != nullptr) return;

		Particle* i = new Particle({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 2.0f, 10, { 1,0,0,1 }, CreateShape(physx::PxSphereGeometry(1 * scale)), false);
		//_registry.addReg(activeForce, i);
		std::shared_ptr<GaussianParticleGenerator> p;
		p.reset(new GaussianParticleGenerator("Gauss", i, 0.75, { 10,10,10 }, { 100,100,100 }, 100));
		Firework* f = new Firework(pos, { 0,20,0 }, {0, (float)scale ,0 }, 0.99, 3.0f, 2, { 0,0,0,1 }, CreateShape(physx::PxSphereGeometry(1 * scale)), { p });
		_particles.push_back(f);
		//_registry.addReg(activeForce, f);
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
		_forceGenerators.push_back(gen);
		
		for (int i = 0; i < num; i++) {
			Particle* part = new Particle({ 10,(float)i,10 }, { 0,0,0 }, { 0,0,0 },
				0.99, 10, 20, { 0,0,1,1 }, CreateShape(physx::PxSphereGeometry(1)), true);
			_registry.addReg(gen, part);
			_particles.push_back(part);
		}

		activeForce = 1;
	}
	void WindGen(int num) {
		
		WindForceGen* gen = new WindForceGen(Vector3(5, 0, 0), 1, 0.1);
		_forceGenerators.push_back(gen);
		
		for (int i = 0; i < num; i++) {
			Particle* part = new Particle({ 10,(float)i,10 }, { 0,0,0 }, { 0,0,0 },
				0.99, 10, 20, { 0,1,0,1 }, CreateShape(physx::PxSphereGeometry(1)), true);
			_registry.addReg(gen, part);
			_particles.push_back(part);
		}
		activeForce = 2;
	}

	void BoatWind(Vector3 dir) {
		activeForce = 2;
		for (auto i = _forceGenerators.begin(); i != _forceGenerators.end(); i++) {
			if (typeid(*i) == typeid(WindForceGen)) {
				_registry.deleteForceReg(*i);
				
				delete (*i);
				i = _forceGenerators.erase(i);
			}
		}
		WindForceGen* gen = new WindForceGen(dir * 100, 1, 0.1);
		_forceGenerators.push_back(gen);

		Particle* i = new Particle({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 1, 5, { 1,1,1,1 }, CreateShape(physx::PxSphereGeometry(1)), false);
		_registry.addReg(gen, i);
		std::shared_ptr<UniformParticleGenerator> p;
		p.reset(new UniformParticleGenerator("Uni", i, 0.3, { 100000,100000,100000 }, { 1,1,1}, 1000));
		Firework* f = new Firework(pos, { 0,0,0 }, { 0,0,0 }, 0., 1.0f, 0, { 1,1,1,1 }, CreateShape(physx::PxBoxGeometry(Vector3(1, 1, 1))), { p });
		_particles.push_back(f);
		_registry.addReg(gen, f);

	}
	
	void TornadoGen(int num) {

		
		TornadoForceGen* gen = new TornadoForceGen(Vector3(0, 0, 0), 1, 0.1, 0.75);
		_forceGenerators.push_back(gen);
		
		for (int i = 0; i < num; i++) {
			Particle* part = new Particle({ 10,(float)i,10}, { 0,0,0 }, { 0,0,0 },
				0.99, 10, 1e10, { 0,0,0,1 }, CreateShape(physx::PxSphereGeometry(1)), true);
			_registry.addReg(gen, part);
			_particles.push_back(part);
		}
		
		activeForce = 3;
		
	}

	void AnchSpringGen() {
		Particle* p = new Particle({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 5, 1e10);
		AnchoredSpringFG* spring = new AnchoredSpringFG(Vector3(-10, 0, 0), 2, 5);
		_registry.addReg(spring, p);
		DragForceGen* drag = new DragForceGen(.5);
		_registry.addReg(drag, p);
		_particles.push_back(p);
		_forceGenerators.push_back(drag);
		_forceGenerators.push_back(spring);
	}
	
	void SpringGen() {
		Particle* p1 = new Particle({ 10,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 5, 1e10);
		Particle* p2 = new Particle({ -10,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 5, 1e10, { 0,0,1,1 });
		SpringForceGen* spring1 = new SpringForceGen(p1, 2, 5);
		SpringForceGen* spring2 = new SpringForceGen(p2, 2, 5);
		_registry.addReg(spring1, p2);
		_registry.addReg(spring2, p1);
		DragForceGen* drag = new DragForceGen(.5);
		_registry.addReg(drag, p1);
		_registry.addReg(drag, p2);
		_particles.push_back(p1);
		_particles.push_back(p2);
		_forceGenerators.push_back(drag);
		_forceGenerators.push_back(spring1);
		_forceGenerators.push_back(spring2);
	}

	void BungeeGen() {
		Particle* p1 = new Particle({ 10,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 5, 1e10);
		Particle* p2 = new Particle({ -10,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 5, 1e10, { 1,0,0,1 });
		BungeeForceGen* spring1 = new BungeeForceGen(p1, 2, 10);
		BungeeForceGen* spring2 = new BungeeForceGen(p2, 2, 10);
		_registry.addReg(spring1, p2);
		_registry.addReg(spring2, p1);
		DragForceGen* drag = new DragForceGen(.05);
		_registry.addReg(drag, p1);
		_registry.addReg(drag, p2);
		_particles.push_back(p1);
		_particles.push_back(p2);
		_forceGenerators.push_back(drag);
		_forceGenerators.push_back(spring1);
		_forceGenerators.push_back(spring2);
	}

	void BuoyancyGen() {
		Particle* liquid = new Particle({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 0, 1e10, { 0,0,0.3,1 }, CreateShape(physx::PxBoxGeometry(20, 2, 20)));
		Particle* p = new Particle({ 0,-5, 0 }, { 0,0,0 }, { 0,0,0 }, 0.99, 5, 1e10, { 1,1,0,1 }, CreateShape(physx::PxBoxGeometry(3, 3, 3)));

		BuoyancyForceGen* bg = new BuoyancyForceGen(5, 27, 1, liquid);
		GravForceGen* gg = new GravForceGen(Vector3(0, -9.8, 0), 0);
		_registry.addReg(bg, p);
		_registry.addReg(gg, p);
		_forceGenerators.push_back(gg);
		_forceGenerators.push_back(bg);
		_particles.push_back(p);
	}
protected:
	std::list<Particle*> _particles;
	std::list<ParticleGenerator*> _generators;
	std::list<ForceGen*> _forceGenerators;
	
	ParticleForceReg _registry;
	
	Vector3 pos;
	int activeForce = 3;
};