#pragma once
#pragma once
#include "ForceGenerator.h"
#include "Particle.h"
#include <map>
typedef std::pair<ForceGen*, Particle*> FRpair;

class ParticleForceReg : public std::multimap<ForceGen*, Particle*>
{
public:
	void updateForces(float duration) {
		for (auto it = begin(); it != end(); ++it)
			it->first->updateForce(it->second, duration);
	}

	void addReg(ForceGen* fg, Particle* p) {
		insert({ fg, p });
	}
	void addReg(int type, Particle* p) {
		ForceGen* fg;
		switch (type) {
		case 0:
			return;
			break;
		case 1:
			fg = new GravForceGen(Vector3(0, -9.8, 0), 100);
			break;
		case 2:
			fg = new WindForceGen(Vector3(5, 0, 0), 1, 0.1);
			break;
		case 3:
			fg = new TornadoForceGen(Vector3(0, 0, 0), 1, 0.1, 0.75);
			break;
		default:
			return;
			break;
		}
		insert({ fg, p });
	}

	void deletePartReg(Particle* p) {
		for (auto it = begin(); it != end();) {
			if (it->second == p) it = erase(it);
			else ++it;
		}
	}
};