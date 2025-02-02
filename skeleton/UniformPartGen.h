#pragma once
#include "ParticleGenerator.h"
#include <random>
class UniformParticleGenerator : public ParticleGenerator {
public:
	UniformParticleGenerator(std::string name, Particle* model, double genProb, Vector3 posDeviation, Vector3 velDeviation, int numPart) {
		setName(name);
		_model = model;
		_mean_pos = model->getPos();
		_mean_vel = model->getVel();
		_mean_acc = model->getAcc();
		_generation_probability = genProb * 100;
		_std_dev_vel = velDeviation;
		_std_dev_pos = posDeviation;
		_num_particles = numPart;
		std::random_device r;
		random_generator = std::mt19937(r());
	}

	std::list<Particle*> generateParticles() override {
		std::list<Particle*> l;
		auto px = std::uniform_real_distribution<float>((float)_mean_pos.x - _std_dev_pos.x, (float)_std_dev_pos.x + _mean_pos.x);
		auto py = std::uniform_real_distribution<float>((float)_mean_pos.y - _std_dev_pos.y, (float)_std_dev_pos.y + _mean_pos.y);
		auto pz = std::uniform_real_distribution<float>((float)_mean_pos.z - _std_dev_pos.z, (float)_std_dev_pos.z + _mean_pos.z);
		auto gen = std::uniform_int_distribution<int>(0, 100);
		auto vx = std::uniform_real_distribution<float>((float)_mean_vel.x - _std_dev_vel.x, (float)_std_dev_vel.x + _mean_pos.x);
		auto vy = std::uniform_real_distribution<float>((float)_mean_vel.y - _std_dev_vel.y, (float)_std_dev_vel.y + _mean_pos.y);
		auto vz = std::uniform_real_distribution<float>((float)_mean_vel.z - _std_dev_vel.z, (float)_std_dev_vel.z + _mean_pos.z);


		for (int i = 0; i < _num_particles; i++) {
			int cr = gen(random_generator);
			if (cr <= _generation_probability)
			{
				Vector3 pos = { px(random_generator), py(random_generator), pz(random_generator) };
				Vector3 vel = { vx(random_generator), vy(random_generator), vz(random_generator) };

				Particle* p = _model->clone();
				p->setPos(_mean_pos);
				p->setVel(vel);
				l.push_back(p);
			}
		}
		return l;
	};
protected:
	std::mt19937 random_generator;
	Vector3 _std_dev_vel, _std_dev_pos;
};