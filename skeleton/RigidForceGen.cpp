#include "RigidForceGen.h"

void RigidWindFG::updateForce(RigidBody* rb, double t)
{
	if (rb->getUnMass() <= 0) return;

	
	
	Vector3 pVel = rb->getLinVel();
	Vector3 newVel = _wind - pVel;

	Vector3 force = _k1 * newVel + _k2 * newVel.magnitude() * newVel;
	rb->addForce(force);
}

void RigidBuoyancy::updateForce(RigidBody* rb, double t)
{
	if (fabs(rb->getUnMass() <= 0)) return;
	
	float depth = rb->getPosition().y;
	if (depth >= _waterHeight - _maxDepth) return;
	
	physx::PxQuat rbRot = rb->getRotation().getNormalized();
	
	Vector3 buoyancyPoints[] = {
		Vector3(-3, 0, 0),
		Vector3(3, 0, 0),
		Vector3(0, 0, 5),
		Vector3(0, 0, -5),
	};
	Vector3 force = Vector3(0, 0, 0);
	
	for (int i = 0; i < 4; i++) {
		Vector3 point = rbRot.rotate(buoyancyPoints[i]);
		
		point = point + rb->getPosition();
		
		force = calcForce(point.y, rb);
		
		rb->addTorque(buoyancyPoints[i].cross(force));
	}
	
	rb->addForce(calcForce(depth, rb));
}

Vector3 RigidBuoyancy::calcForce(float depth, RigidBody* rb)
{
	Vector3 force(0, 0, 0);

	if (depth <= _waterHeight - _maxDepth) {
		force.y = (_liquidDensity * _volume);
		return force;
	}
	
	force.y = (_liquidDensity * _volume * (depth - _maxDepth - _waterHeight) * 9.8 / 2 * _maxDepth);
	
	return	force;
}


