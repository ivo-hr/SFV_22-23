#include "Particle.h"

Particle::Particle(Vector3 Pos, Vector3 Vel) {

	this->vel = Vel;
	this->pose = physx::PxTransform(Pos);
	physx::PxShape* sph = CreateShape(physx::PxSphereGeometry(1));
	this->renderItem = new RenderItem(sph, &this->pose, Vector4(1, 0.7, 0.8, 1));
}

Particle::~Particle() {
	renderItem->release();
}

void Particle::integrate(double t) {
	this->pose.p += this->vel * t;
}