#include "RigidBody.h"
#include <iostream>

RigidBody::RigidBody(physx::PxRigidDynamic* rigid, double time, Vector4 color) {
	_rigid = rigid;
	_lifeTime = time;
	_life = 0;
	physx::PxShape* shape;
	_rigid->getShapes(&shape, 1);
	_render_item = new RenderItem(shape, _rigid, color);
}

RigidBody::RigidBody(physx::PxScene* scene, physx::PxPhysics* physx, Vector3 pos, Vector3 vel, double mass, double time, Vector3 dims, Vector4 color, Type type)
{
	_rigid = physx->createRigidDynamic(physx::PxTransform(pos));
	_rigid->setLinearVelocity(vel);
	_rigid->setMass(mass);
	_rigid->setAngularVelocity({ 0,0,0 });
	_lifeTime = time;
	_life = 0;
	
	physx::PxShape* sh;
	switch (type)
	{
	case Cube:
		sh = CreateShape(physx::PxBoxGeometry(dims));
		break;
		
	case Capsule:
		sh = CreateShape(physx::PxCapsuleGeometry(dims.x, dims.y));
		break;
		
	case Sphere:
		sh = CreateShape(physx::PxSphereGeometry(dims.x));
		break;
		
	default:
		sh = CreateShape(physx::PxBoxGeometry(dims));
		break;
		
	}
	
	_rigid->attachShape(*sh);
	physx::PxRigidBodyExt::updateMassAndInertia(*_rigid, 1);
	scene->addActor(*_rigid);
	_render_item = new RenderItem(sh, _rigid, color);
}

RigidBody::~RigidBody() {
	_rigid->release();
	DeregisterRenderItem(_render_item);
}
