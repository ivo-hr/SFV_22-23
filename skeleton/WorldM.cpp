#include "WorldM.h"

WorldManager::WorldManager(physx::PxScene* scene, physx::PxPhysics* physics)
{
	gScene = scene;
	gPhysics = physics;

	/*suelo = gPhysics->createRigidStatic(physx::PxTransform({ 0, 0, 0 }));
	physx::PxShape* shape = CreateShape(physx::PxBoxGeometry(Vector3(25, 1, 25)));
	suelo->attachShape(*shape);
	scene->addActor(*suelo);
	sueloRender = new RenderItem(shape, suelo, { 0.3, 0.3, 0.3, 0 });*/
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


	
};


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

void WorldManager::GenBoatGame() {

	//mar
	/*physx::PxRigidStatic* mar = gPhysics->createRigidStatic(physx::PxTransform({ 0, -1, 0 }));
	physx::PxShape* shape = CreateShape(physx::PxBoxGeometry(Vector3(25, 1, 25)));
	mar->attachShape(*shape);
	gScene->addActor(*mar);
	RenderItem* marRender = new RenderItem(shape, mar, { 0.3, 0.3, 1, 0.6 });*/
	
	//barco
	physx::PxRigidDynamic* barco = gPhysics->createRigidDynamic(physx::PxTransform({ 0, 0, 0 }));
	barco->setName("barco");
	physx::PxRigidBodyExt::updateMassAndInertia(*barco, 200);
	physx::PxShape* boatShape = CreateShape(physx::PxBoxGeometry(Vector3(10, 5, 20)));
	barco->attachShape(*boatShape);
	gScene->addActor(*barco);
	RenderItem* boatRender = new RenderItem(boatShape, barco, { 0.3, 0.3, 0.3, 1});
	boat = new RigidBody(barco, boatRender->color);

	//vela
	physx::PxRigidDynamic* vela = gPhysics->createRigidDynamic(physx::PxTransform({ 0, 30, 0 }));
	vela->setName("barco");
	physx::PxRigidBodyExt::updateMassAndInertia(*vela, 1);
	physx::PxShape* sailShape = CreateShape(physx::PxBoxGeometry(Vector3(20, 10, 0.1)));
	vela->attachShape(*sailShape);
	vela->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	
	gScene->addActor(*vela);
	RenderItem* sailRender = new RenderItem(sailShape, vela, { 0.8, 0.9, 0.7, 1 });
	sail = new RigidBody(vela, sailRender->color);
	//wind
	windFG = new RigidWindFG({ -0,0, 0 }, 1, .5);
	_forceGens.push_back(windFG);
	_forceRegs.addRigidReg(windFG, sail);
	_forceRegs.addRigidReg(windFG, boat);

	//buoyancy
	RigidBuoyancy* buoyancy = new RigidBuoyancy(1, 1, 0, 60);
	_forceGens.push_back(buoyancy);
	_forceRegs.addRigidReg(buoyancy, boat);


	

	//Muros
	physx::PxRigidStatic* muro1 = gPhysics->createRigidStatic(physx::PxTransform({ 0, 0, -4000 }));
	muro1->setName("muro");
	physx::PxShape* shape = CreateShape(physx::PxBoxGeometry(Vector3(10000, 500, 10)));
	muro1->attachShape(*shape);
	muro1->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	gScene->addActor(*muro1);
	RenderItem* muro1Render = new RenderItem(shape, muro1, { 1, 0.3, 0.3, 1 });

	physx::PxRigidStatic* muro2 = gPhysics->createRigidStatic(physx::PxTransform({ 700, 0, 0 }));
	muro2->setName("muro");
	physx::PxShape* shape2 = CreateShape(physx::PxBoxGeometry(Vector3(10, 500, 10000)));
	muro2->attachShape(*shape2);
	muro2->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	gScene->addActor(*muro2);
	RenderItem* muro2Render = new RenderItem(shape2, muro2, { 1, 0.3, 0.3, 1 });
	
	physx::PxRigidStatic* muro3 = gPhysics->createRigidStatic(physx::PxTransform({ -700, 0, 0 }));
	muro3->setName("muro");
	physx::PxShape* shape3 = CreateShape(physx::PxBoxGeometry(Vector3(10, 500, 10000)));
	muro3->attachShape(*shape3);
	muro3->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	gScene->addActor(*muro3);
	RenderItem* muro3Render = new RenderItem(shape3, muro3, { 1, 0.3, 0.3, 1 });
	
	physx::PxRigidStatic* muro4 = gPhysics->createRigidStatic(physx::PxTransform({ 250, 0, -1300}));
	muro4->setName("muro");
	physx::PxShape* shape4 = CreateShape(physx::PxBoxGeometry(Vector3(250, 250, 250)));
	muro4->attachShape(*shape4);
	muro4->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	gScene->addActor(*muro4);
	RenderItem* muro4Render = new RenderItem(shape4, muro4, { 0, 0.3, 0.3, 1 });
	
	physx::PxRigidStatic* muro5 = gPhysics->createRigidStatic(physx::PxTransform({ -500, 0, -1000 }));
	muro5->setName("muro");
	physx::PxShape* shape5 = CreateShape(physx::PxBoxGeometry(Vector3(100, 100, 100)));
	muro5->attachShape(*shape5);
	muro5->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	gScene->addActor(*muro5);
	RenderItem* muro5Render = new RenderItem(shape5, muro5, { 1, 0.3, 0.4, 1 });

	physx::PxRigidStatic* muro6 = gPhysics->createRigidStatic(physx::PxTransform({ 500, 0, -300 }));
	muro6->setName("muro");
	physx::PxShape* shape6 = CreateShape(physx::PxBoxGeometry(Vector3(100, 100, 100)));
	muro6->attachShape(*shape6);
	muro6->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	gScene->addActor(*muro6);
	RenderItem* muro6Render = new RenderItem(shape6, muro6, { 1, 0.7, 0.3, 1 });
	
	
	//moneda
	physx::PxRigidDynamic* moneda = gPhysics->createRigidDynamic(physx::PxTransform({ -250, 60, -1500 }));
	moneda->setName("moneda");
	physx::PxShape* coinShape = CreateShape(physx::PxBoxGeometry(50, 50, 1));
	moneda->attachShape(*coinShape);
	moneda->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	gScene->addActor(*moneda);
	RenderItem* coinRender = new RenderItem(coinShape, moneda, { 1, 1, 0.0, 1 });
	coin = new RigidBody(moneda, coinRender->color);

	coin->addTorque({ 0, 10, 0 });
	
	
	
	
}