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
	void changeWind(Vector3& v) { _windVel = v; };
	void addWind(Vector3& v) { _windVel += v; };
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

class SpringForceGen : public ForceGen {
public:
	SpringForceGen(Particle* other, float k, float resting_length);
	virtual ~SpringForceGen() {};
	void updateForce(Particle* p, double duration) override;

protected:
	float _k;
	float _resting_length;
	Particle* _other;
};

class AnchoredSpringFG : public SpringForceGen {
public:
	AnchoredSpringFG(Vector3& position, float k, float resting_length);
	virtual ~AnchoredSpringFG();
};

class BungeeForceGen : public SpringForceGen {
public:
	BungeeForceGen(Particle* other, float k, float resting_length);
	virtual ~BungeeForceGen() {};
	void updateForce(Particle* p, double duration) override;
};

class BuoyancyForceGen : public ForceGen {
public:
	BuoyancyForceGen(float height, float V, float d, Particle* liquid_surface);
	virtual ~BuoyancyForceGen() {};
	void updateForce(Particle* p, double duration) override;

protected:
	float _height;
	float _volume;
	float _density;
	Particle* _liquid_particle;
};