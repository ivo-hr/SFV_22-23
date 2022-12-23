#include "RigidForceGen.h"

void RigidWindFG::updateForce(RigidBody* rb, double t)
{
	if (rb->getUnMass() <= 0) return;

	Vector3 pVel = rb->getLinVel();
	Vector3 newVel = _wind - pVel;

	Vector3 force = _k1 * newVel + _k2 * newVel.magnitude() * newVel;
	rb->addForce(force);
}