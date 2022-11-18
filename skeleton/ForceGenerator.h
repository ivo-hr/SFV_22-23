#pragma once
#include "Particle.h"
#include <list>
#include <string>

class ForceGen
{
public:
	virtual void updateForce(Particle* particle, double duration) = 0;
	std::string name;
	double t = -1;
};

class GravForceGen : public ForceGen {
public:
	GravForceGen(Vector3& G, double t);
	virtual ~GravForceGen() {};
	void updateForce(Particle* particle, double duration) override;
protected:
	Vector3 g;
};

class DragForceGen : public ForceGen {
public:
	DragForceGen(float k) : k(k) {};
	virtual ~DragForceGen() {};
	void updateForce(Particle* particle, double duration) override;
protected:
	float k;
};

class WindForceGen : public ForceGen {
public:
	WindForceGen(Vector3& v, float k1, float k2);
	virtual ~WindForceGen() {};
	void updateForce(Particle* particle, double duration) override;
protected:
	Vector3 _windVel;
	float _k1;
	float _k2;
};

class TornadoForceGen : public WindForceGen {
public:
	TornadoForceGen(Vector3 center, float k1, float k2, float kt) : WindForceGen(Vector3(0), k1, k2), _center(center), _Kt(kt) {};
	~TornadoForceGen() {};
	void updateForce(Particle* particle, double duration) override;
protected:
	Vector3 _center;
	float _Kt;
};