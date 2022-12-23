#pragma once
#include <string>
#include "core.hpp"
#include "RigidBody.h"

class RigidForceGen
{
public:
	virtual void updateForce(RigidBody* rb, double t) = 0;
	std::string name;
	double t = -1;
};

class RigidWindFG : public RigidForceGen {
public:
	RigidWindFG(Vector3 wind, float k1, float k2) {
		_k1 = k1;
		_k2 = k2;
		_wind = wind;
		name = "Wind";
	};
	void updateForce(RigidBody* rb, double t) override;
protected:
	float _k1, _k2;
	Vector3 _wind;
};