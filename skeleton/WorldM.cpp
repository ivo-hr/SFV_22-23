#include "WorldM.h"

WorldManager::WorldManager(physx::PxScene* scene, physx::PxPhysics* physics)
{
	gScene = scene;
	gPhysics = physics;

	suelo = gPhysics->createRigidStatic(physx::PxTransform({ 0, 0, 0 }));
	physx::PxShape* shape = CreateShape(physx::PxBoxGeometry(Vector3(25, 1, 25)));
	suelo->attachShape(*shape);
	scene->addActor(*suelo);
	sueloRender = new RenderItem(shape, suelo, { 0.3, 0.3, 0.3, 0 });
}

WorldManager::~WorldManager()
{
	for (auto it = _forceGens.begin(); it != _forceGens.end(); it = _forceGens.erase(it))
		delete* it;
	
	for (auto it = _RBGens.begin(); it != _RBGens.end(); it = _RBGens.erase(it))
		delete* it;
	
	for (auto it = _rigids.begin(); it != _rigids.end(); it = _rigids.erase(it))
		delete* it;
}

void WorldManager::update(double t) {
	for (auto gen : _RBGens) {
		RigidBody* rb = gen->generateParticles(t);
		if (rb != nullptr)
		{
			_rigids.push_back(rb);
			for (auto force : _forceGens) {
				_forceRegs.addRigidReg(force, rb);
			}
		}
	}
	_forceRegs.updateForces(t);
	for (auto it = _rigids.begin(); it != _rigids.end(); ) {
		(*it)->update(t);
		if (!(*it)->isAlive()) {
			_forceRegs.deleteRigidReg(*it);
			delete (*it);
			it = _rigids.erase(it);
		}
		else {
			++it;
		}
	}
}

void WorldManager::GenDemo() {


	
	GaussRigidGen::Config c;
	c._mean_pos = { 0,10,0 };
	c._dev_pos = { 10,0,0 };
	c._shape = CreateShape(physx::PxCapsuleGeometry(2,1));
	c._color = { 0,0,1,1 };

	GaussRigidGen* s = new GaussRigidGen(c, gScene, gPhysics);

	_RBGens.push_back(s);

	RigidWindFG* wind = new RigidWindFG({ -4,0, 2 }, 1, .5);
	_forceGens.push_back(wind);
}