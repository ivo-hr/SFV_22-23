#include "Particle.h"

Particle::Particle(Vector3 Pos, Vector3 Vel, Vector3 Acc, float Damp) {

	this->vel = Vel;
	this->acc = Acc;
	this->damp = Damp;
	this->pose = physx::PxTransform(Pos);
	physx::PxShape* sph = CreateShape(physx::PxSphereGeometry(1));
	this->renderItem = new RenderItem(sph, &this->pose, Vector4(1, 0.2, 0.2, 1));
}

Particle::Particle(Vector3 Pos, Vector3 Scale) {
	this->vel = Vector3();
	this->acc = Vector3();
	this->damp = 0;
	this->pose = physx::PxTransform(Pos);

	physx::PxShape* pln = CreateShape(physx::PxBoxGeometry(Scale.x, Scale.y, Scale.z));
	this->renderItem = new RenderItem(pln, &this->pose, Vector4(0.1, 0.1, 0.1, 1));
}


Particle::~Particle() {
	renderItem->release();
}

void Particle::integrate(double t) {
	this->pose.p += this->vel * t;

	this->vel = this->vel * pow(damp, t) + acc * t;
}