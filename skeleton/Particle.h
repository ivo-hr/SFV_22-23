#pragma once

#include "core.hpp"
#include "RenderUtils.hpp"

class Particle
{
public:
	Particle(Vector3 Pos, Vector3 Vel, Vector3 Acc, float damp, float mass, float time = 5.0f, Vector4 color = { 1,1,1,1 }, physx::PxShape* sh = CreateShape(physx::PxSphereGeometry(1)), bool render = true);

	virtual ~Particle();

	virtual void integrate(double t);
	virtual bool isAlive() { return timeLimit > 0; }
	virtual Particle* clone() {
		return new Particle(pose.p, vel, acc, damping, inverse_mass > 0 ? 1.0f / inverse_mass : 0.0f, timeLimit, color, shape);
	};
	virtual void setVel(Vector3 vel) { this->vel = vel; };
	virtual void setPos(Vector3 pos) { this->pose.p = pos; };
	virtual void setAcc(Vector3 acc) { this->acc = acc; };
	virtual void setMass(float mass) {
		if (mass <= 0) return;
		inverse_mass = 1.0f / mass;
	}
	virtual void clearForce() { force *= 0; }
	virtual void addForce(Vector3 f) { force += f; }
	virtual Vector3 getPos() { return this->pose.p; };
	virtual Vector3 getVel() { return this->vel; };
	virtual Vector3 getAcc() { return this->acc; };
protected:
	Vector4 color;
	physx::PxShape* shape;
	Vector3 vel, acc;
	Vector3 force;
	float inverse_mass;
	float damping;
	physx::PxTransform pose;
	RenderItem* renderItem;
	float timeLimit;
};

class Floor : public Particle {
public:
	Floor(Vector3 Pos, Vector3 Dim) : Particle(Pos, Vector3(0, 0, 0), Vector3(0, 0, 0), 0.0f, 1.0f, 1, { 0.9,0.5,0.9,1 }, CreateShape(physx::PxBoxGeometry(Dim))) {
	}
	virtual ~Floor() {};
};

