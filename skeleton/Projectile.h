#pragma once

#include <string>
#include "Particle.h"
#include "RenderUtils.hpp"
class Projectile:public Particle
{
public:

	enum Type { Bullet, Mortar, RPG, Plasma };

	Projectile(Type type) : Particle(GetCamera()->getEye(), Vector3(), Vector3(), 0) {

		Vector3 dir = GetCamera()->getDir();

	switch (type)
		{
		case Projectile::Bullet:

			vel = 300 * dir;
			acc = Vector3(0, -9.8, 0);
			damp = 0.9;

			break;
		case Projectile::Mortar:

			vel = 40 * dir;
			acc = Vector3(0, -9.8, 0);
			damp = 0.9;

			break;
		case Projectile::RPG:

			vel = 60 * dir;
			acc = Vector3(0, -9.8, 0) + dir * 50;
			damp = 0;

			break;
		case Projectile::Plasma:
			break;
		default:
			vel = 20 * dir;
			acc = Vector3(0, +1.2, 0) + dir * 6;
			damp = 0.9;
			break;
		}

	}

private:
	Vector3 pos;
	//new Particle(Vector3(-30, -30, 0), Vector3(10, 20, 0), Vector3(0, -9.8, 0), 0.9);



};

