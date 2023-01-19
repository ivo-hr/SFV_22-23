#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"

class RigidBody
{
public:

	enum Type { Cube, Capsule, Sphere };
	
	RigidBody(physx::PxRigidDynamic* rigid, double time, Vector4 color);
	RigidBody(physx::PxRigidDynamic* rigid, Vector4 color);
	RigidBody(physx::PxScene* scene, physx::PxPhysics*, Vector3 pos, Vector3 vel, double mass, double time, Vector3 dims, Vector4 color, Type type = Cube);
	virtual ~RigidBody();
	void update(double t) {
		if(_life != -1) 
			if (_lifeTime > 0.0) _life += t;
	};
	bool isAlive() {
		return _life < _lifeTime;
	};

	void addForce(Vector3 f) {
		_rigid->addForce(f);
	};
	void addTorque(Vector3 f) {
		_rigid->addTorque(f);
	};

	Vector3 getPosition() const {
		return _rigid->getGlobalPose().p;
	};
	physx::PxQuat getRotation() const {
		return _rigid->getGlobalPose().q;
	};
	Vector3 getLinVel() const {
		return _rigid->getLinearVelocity();
	};
	float getMass() const {
		return _rigid->getMass();
	};
	float getUnMass() const {
		if (_rigid->getMass() <= 0) return -1;
		return _rigid->getInvMass();
	};
	void setRotation(physx::PxQuat q) {
		_rigid->setGlobalPose(physx::PxTransform(getPosition(), q));
	};

	


protected:
	physx::PxRigidDynamic* _rigid;
	RenderItem* _render_item;
	double _lifeTime;
	double _life;
};