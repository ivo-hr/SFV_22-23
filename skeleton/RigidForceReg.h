#pragma once
#include <map>
#include "RigidForceGen.h"
#include "RigidBody.h"
#include "core.hpp"

class RBForceRegistry : std::multimap<RigidForceGen*, RigidBody*>
{
public:
	void updateForces(float duration) {
		for (auto it = begin(); it != end(); ++it)
			it->first->updateForce(it->second, duration);
	}

	void addRigidReg(RigidForceGen* fg, RigidBody* rg) {
		insert({ fg, rg });
	}

	void deleteRigidReg(RigidBody* rb) {
		for (auto it = begin(); it != end();) {
			if (it->second == rb) it = erase(it);
			else ++it;
		}
	}
};