#pragma once

#include "core.hpp"
#include "RenderUtils.hpp"

class Particle
{
public:
	Particle(Vector3 Pos, Vector3 Vel, Vector3 Acc, Vector3 Scale, Vector3 Colour, float Damp);
	Particle(Vector3 Pos, Vector3 Scale);
	~Particle();

	void integrate(double t);

protected:
	Vector3 vel, acc, scal;
	float damp;
	physx::PxTransform pose;
	RenderItem* renderItem;
};

