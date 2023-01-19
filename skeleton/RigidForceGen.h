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
	void changeWind(Vector3 wind) {
		_wind = wind;
	}
	void addWind(Vector3 wind) {
		if(0 < wind.x < 10)
		_wind.x += wind.x;
		if (0 < wind.y < 10)
			_wind.y += wind.y;
		if (0 < wind.z < 10)
			_wind.z += wind.z;
	}
	Vector3 getWind() {
		return _wind;
	}
	void updateForce(RigidBody* rb, double t) override;
protected:
	float _k1, _k2;
	Vector3 _wind;
};

class RigidBuoyancy : public RigidForceGen {
public:
	RigidBuoyancy(float maxDepth, float volume, float waterHeight, float liquidDensity = 1000.0f, float viscosity = 0.5f) {
		_maxDepth = maxDepth;
		_volume = volume;
		_waterHeight = waterHeight;
		_liquidDensity = liquidDensity;
		_viscosity = viscosity;
		name = "Buoyancy";
	};
	void updateForce(RigidBody* rb, double t) override;
	Vector3 calcForce(float depth, RigidBody* rb);

protected:
	float _maxDepth, _volume, _waterHeight, _liquidDensity, _viscosity;

};