#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>

#include "Particle.h"

#include "Projectile.h"

#include "ParticleSystem.h"

#include "WorldM.h"
#include "RigidBody.h"


using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;


Particle* flr;

std::vector<Projectile*> projectiles;

ParticleSystem* partSys;
ParticleSystem* botDeth;
RigidBody* rb;
WorldManager* world;

Camera* cam;

bool ended = false;

// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	flr = new Floor(Vector3(0, 0, 0), Vector3(10000, 0.01, 10000));

	partSys = new ParticleSystem(Vector3(0, 30, 0));

	world = new WorldManager(gScene, gPhysics);

	//partSys->AnchSpringGen();
	//partSys->SpringGen();
	//partSys->BungeeGen();
	//partSys->BuoyancyGen();
	//partSys->gravGen(100);
	//partSys->TornadoGen(100);
	//partSys->WindGen(100);

	//partSys->generateGFireworksSystem();
	
	//world->GenDemo();


	world->GenBoatGame();

	cam = GetCamera();
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	
	
	
	
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	/*for (Projectile* p : projectiles) {
		p->integrate(t);

		p->restLifeTime(t);
		if (p->getLifeTime() < 0) {
			delete p;
		}

	}*/

	for (auto i = projectiles.begin(); i != projectiles.end();) {
		(*i)->integrate(t);
		
		(*i)->restLifeTime(t);

		if ((*i)->getLifeTime() < 0) {
			(*i)->clearForce();
			delete (*i);
			(i) = projectiles.erase(i);
		}
		else ++i;
	}

	partSys->update(t);
	
	world->update(t);

	if (botDeth != NULL)	botDeth->update(t);
	
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{


	PX_UNUSED(interactive);

	for (Projectile* p : projectiles) {
		delete p;
	}
	delete partSys;
	if (botDeth != NULL) delete botDeth;
	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();

	delete rb;

	delete world;

	}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch(toupper(key))
	{
	case ' ':
	{
		world->windFG->changeWind(Vector3(0, 0, 0));
		partSys->BoatWind(Vector3(0, 0, 0));
		//camera.transform(PxVec3(0, 100000, 0));
		//cam->getDir() = Vector3(0, 0, 0);
		if (ended) {
			/*cleanupPhysics(true);
			
			initPhysics(true);*/
		}

		break;
	}
	case 'T':
		world->windFG->addWind(Vector3(0, 0, -1));
		
		
		/*if (world->boat->getRotation().y < physx::PxQuat(physx::PxPi / 2 + 1, physx::PxVec3(0, 1, 0)).y) {
			world->boat->addTorque(Vector3(0, 1, 0));
			world->sail->addTorque(Vector3(0, 1, 0));
		}
		else if (world->boat->getRotation().y > physx::PxQuat(physx::PxPi / 2 - 1, physx::PxVec3(0, 1, 0)).y) {
			world->boat->addTorque(Vector3(0, -1, 0));
			world->sail->addTorque(Vector3(0, -1, 0));
		}*/
			break;
	case 'F':
		world->windFG->addWind(Vector3(-1, 0, 0));
		/*if (world->boat->getRotation().y < physx::PxQuat(physx::PxPi, physx::PxVec3(0, 1, 0)).y) {
			world->boat->addTorque(Vector3(0, 1, 0));
			world->sail->addTorque(Vector3(0, 1, 0));
		}
		else if (world->boat->getRotation().y > physx::PxQuat(physx::PxPi, physx::PxVec3(0, 1, 0)).y) {
			world->boat->addTorque(Vector3(0, -1, 0));
			world->sail->addTorque(Vector3(0, -1, 0));
		}*/
		break;
	case 'G':
		world->windFG->addWind(Vector3(0, 0, 1));
		/*if (world->boat->getRotation().y < physx::PxQuat(physx::PxPi * 1.5, physx::PxVec3(0, 1, 0)).y) {
			world->boat->addTorque(Vector3(0, 1, 0));
			world->sail->addTorque(Vector3(0, 1, 0));
		}
		else if (world->boat->getRotation().y > physx::PxQuat(physx::PxPi * 1.5, physx::PxVec3(0, 1, 0)).y) {
			world->boat->addTorque(Vector3(0, -1, 0));
			world->sail->addTorque(Vector3(0, -1, 0));
		}*/
		break;
	case 'H':
		world->windFG->addWind(Vector3(1, 0, 0));
		/*if (world->boat->getRotation().y < physx::PxQuat(0, physx::PxVec3(0, 1, 0)).y) {
			world->boat->addTorque(Vector3(0, 1, 0));
			world->sail->addTorque(Vector3(0, 1, 0));
		}
		else if (world->boat->getRotation().y > physx::PxQuat(0, physx::PxVec3(0, 1, 0)).y) {
			world->boat->addTorque(Vector3(0, -1, 0));
			world->sail->addTorque(Vector3(0, -1, 0));
		}*/
		break;

	default:
		world->windFG->addWind(Vector3(-.1, 0, -.1));
		break;
	}
}



void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	if (!ended) {
		if (actor1->getName() == "barco" && actor2->getName() == "muro" || actor1->getName() == "muro" && actor2->getName() == "barco")
		{
			std::cout << "Collision" << std::endl;
			if (botDeth == NULL) botDeth = new ParticleSystem(world->boat->getPosition());
			botDeth->WindGen(0);
			botDeth->generateGFireworksSystem(20);
			ended = true;
		}

		if (actor1->getName() == "barco" && actor2->getName() == "moneda" || actor1->getName() == "moneda" && actor2->getName() == "barco") {
			std::cout << "Win" << std::endl;
			if (botDeth == NULL) botDeth = new ParticleSystem(world->boat->getPosition());

			botDeth->WindGen(0);
			botDeth->generateNFireworksSystem(20);

			ended = true;
		}
	}
}


int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}