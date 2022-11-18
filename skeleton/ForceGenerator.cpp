#include "ForceGenerator.h"

GravForceGen::GravForceGen(Vector3& G, double t) {
	g = G;
	this->t = t;
}

void GravForceGen::updateForce(Particle* particula, double duration) {
	if (particula->getUnMass() <= 0.0f)
		return;

	particula->addForce(g * particula->getMass());
}

WindForceGen::WindForceGen(Vector3& v, float k1, float k2) : _windVel(v), _k1(k1), _k2(k2)
{
}

void WindForceGen::updateForce(Particle* particle, double duration)
{
	if (particle->getUnMass() <= 0.0f) return;

	Vector3 pVel = particle->getVel();
	Vector3 diffVel = _windVel - pVel;

	Vector3 force = _k1 * diffVel + _k2 * diffVel.magnitude() * diffVel;
	particle->addForce(force);
}

void DragForceGen::updateForce(Particle* particle, double duration)
{
	if (particle->getUnMass() <= 0.0f) return;

	Vector3 force = k * particle->getVel();
	particle->addForce(force);
}

void TornadoForceGen::updateForce(Particle* particle, double duration)
{
	Vector3 pos = particle->getPos();
	Vector3 dist = pos - _center;
	_windVel = _Kt * Vector3(-(dist.z) - (dist.x), 50 - (dist.y), dist.x - (dist.z));
	WindForceGen::updateForce(particle, duration);
}