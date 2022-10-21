#pragma once

#include "core.hpp"
#include "RenderUtils.hpp"

class Particle
{
public:
	Particle(Vector3 Pos, Vector3 Vel, Vector3 Acc, float Damp);
	Particle(Vector3 Pos, Vector3 Vel, Vector3 Acc, Vector4 Col, float Damp, double life);
	Particle(Vector3 Pos, Vector3 Scale);
	~Particle();

	void integrate(double t);

	int getLifeTime() {return lifeTime;}

	void setLifeTime(double set) {lifeTime = set;}

	void restLifeTime(double rest) {lifeTime -= rest;}

protected:
	Vector3 vel, acc, scal;
	float damp;
	double lifeTime = 5;
	physx::PxTransform pose;
	RenderItem* renderItem;
};

