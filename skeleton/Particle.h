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
		return new Particle(pose.p, vel, acc, damping, unMass > 0 ? 1.0f / unMass : 0.0f, timeLimit, color, shape);
	};
	virtual void setVel(Vector3 vel) { this->vel = vel; };
	virtual void setPos(Vector3 pos) { this->pose.p = pos; };
	virtual void setAcc(Vector3 acc) { this->acc = acc; };
	virtual void setMass(float mass) {
		if (mass <= 0) this->unMass = 0;
		else this->unMass = 1/mass;

		this->Mass = mass;
	}
	virtual void clearForce() { force *= 0; }
	virtual void addForce(Vector3 f) { force += f; }
	virtual Vector3 getPos() { return this->pose.p; };
	virtual Vector3 getVel() { return this->vel; };
	virtual Vector3 getAcc() { return this->acc; };
	virtual float getMass() { return this->Mass; };
	virtual float getUnMass() { return this->unMass; };
protected:
	Vector4 color;
	physx::PxShape* shape;
	Vector3 vel, acc;
	Vector3 force;
	float Mass, unMass;
	float damping;
	physx::PxTransform pose;
	RenderItem* renderItem;
	float timeLimit;
};

class Floor : public Particle {
public:
	Floor(Vector3 Pos, Vector3 Dim) : Particle(Pos, Vector3(0, 0, 0), Vector3(0, 0, 0), 0.0f, 1.0f, 1, { 0.,1.,.3,1 }, CreateShape(physx::PxBoxGeometry(Dim))) {
	}
	virtual ~Floor() {};
};

