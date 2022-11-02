#pragma once
#include <list>
#include <vector>
#include <random>
#include "ParticleGenerator.h"

class UniformPartGen : public ParticleGenerator {
public:
	UniformPartGen(Vector3 Pos, Vector3 Vel, Vector3 Acc, Vector4 Col, float Damp, double life, double probability, int maximum) {
		avgPos = Pos; avgVel = Vel; avgAcc = Acc; col = Col; damp = Damp; avgTim = life; genProb = probability; maxPart = maximum;
	}

	std::list<Particle*> generateParticles() override {
		std::list<Particle*> part;

		auto posX = std::uniform_real_distribution<float>(avgPos.x, devPos.z);
		auto posY = std::uniform_real_distribution<float>(avgPos.y, devPos.z);
		auto posZ = std::uniform_real_distribution<float>(avgPos.z, devPos.z);

		auto gen = std::uniform_int_distribution<int>(0, 100);

		auto velX = std::uniform_real_distribution<float>(avgVel.x, devVel.x);
		auto velY = std::uniform_real_distribution<float>(avgVel.y, devVel.y);
		auto velZ = std::uniform_real_distribution<float>(avgVel.z, devVel.z);

		for (int i = 0; i < maxPart; i++) {
			int r = gen(randGen);
			if (r < genProb) {
				Vector3 pos = { posX(randGen), posY(randGen), posZ(randGen) };
				Vector3 vel = { velX(randGen), velY(randGen), velZ(randGen) };
				double lifeTime = std::uniform_int_distribution<int>(avgTim - 2, avgTim + 2)(randGen);

				Particle* p = new Particle(pos, vel, avgAcc, col, damp, lifeTime);

				part.push_back(p);
			}
		}

		return part;
	}
protected:
	Vector3 devPos, devVel;
	std::mt19937 randGen;
};