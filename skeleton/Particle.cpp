#include "Particle.h"

Particle::Particle(Vector3 Pos, Vector3 Vel, Vector3 Acc, float damp, float mass, float time, Vector4 color, physx::PxShape* sh, bool render) {
	this->vel = Vel;
	this->pose = physx::PxTransform(Pos);
	this->acc = Acc;
	this->damping = damp;
	this->color = color;
	this->shape = sh;
	if (render) {
		this->renderItem = new RenderItem(sh, &this->pose, color);
	}
	else {
		this->renderItem = nullptr;
	}
	this->timeLimit = time;
	this->force = { 0,0,0 };
	mass > 0.0f ? this->inverse_mass = 1.0f / mass : this->inverse_mass = 0.0f;
}

Particle::~Particle() {
	DeregisterRenderItem(this->renderItem);
}

void Particle::integrate(double t) {
	if (inverse_mass <= 0.0f) return;
	this->pose.p += this->vel * t;
	this->acc += force * inverse_mass;
	this->vel = this->vel * pow(this->damping, t) + this->acc * t;
	this->timeLimit -= t;
	clearForce();
}