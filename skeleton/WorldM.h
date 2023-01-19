#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include <list>
#include "RigidBody.h"
#include "RigidBodyGen.h"
#include "RigidForceGen.h"
#include "RigidForceReg.h"
#include <random>

class WorldManager
{
public:
	WorldManager(physx::PxScene* scene, physx::PxPhysics* physics);
	void update(double t);
	void GenDemo();
	void GenBoatGame();
	~WorldManager();

	RigidWindFG* windFG;


	RigidBody* boat;
	RigidBody* sail;
	RigidBody* coin;
protected:
	physx::PxScene* gScene;
	physx::PxPhysics* gPhysics;
	std::list<RigidBody*> _rigids;
	std::list<RigidForceGen*> _forceGens;
	std::list<RigidBodyGen*> _RBGens;
	RBForceRegistry _forceRegs;
	RigidBodyGen* gen = nullptr;
	physx::PxRigidStatic* suelo;
	RenderItem* sueloRender;
	
};