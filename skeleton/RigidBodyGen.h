#pragma once
#include "RigidBody.h"
#include "core.hpp"
#include <list>
#include <string>
#include <random>

class RigidBodyGen
{
public:
	struct Config {
		Vector3 _mean_pos = { 0, 0, 0 };
		Vector3 _mean_vel = { 0, 0 ,0 };
		Vector4 _color = { 1, 1, 1, 1 };
		double _generation_time = 2;
		double _rigid_lifetime = 10;
		std::string _name = "";
		physx::PxShape* _shape = CreateShape(physx::PxSphereGeometry(1));
	};
	virtual RigidBody* generateParticles(double t) = 0;
	virtual void init(Config& c) {
		_mean_pos = c._mean_pos;
		_mean_vel = c._mean_vel;
		_modelColor = c._color;
		_generation_time = c._generation_time;
		_timer = _generation_time;
		_rigid_lifetime = c._rigid_lifetime;
		_name = c._name;
		_shape = c._shape;
		active = true;
	};
	
	std::string getName() const { return _name; }
	void setName(std::string name) { _name = name; }
	
	void setActive(bool ac) { 
		active = ac; 
	};
	virtual ~RigidBodyGen() {};

protected:
	Vector3 _mean_pos, _mean_vel;
	Vector4 _modelColor;
	double _generation_time;
	double _timer;
	double _rigid_lifetime;
	std::string _name;
	physx::PxShape* _shape;
	bool active = true;
	physx::PxScene* gScene;
	physx::PxPhysics* gPhysics;
};

class GaussRigidGen : public RigidBodyGen {

public:
	struct Config : RigidBodyGen::Config {
		Vector3 _dev_pos = { 0, 0, 0 };
		Vector3 _dev_vel = { 0, 0, 0 };
	};

	GaussRigidGen(Config& c, physx::PxScene* _scene, physx::PxPhysics* _physics);
	~GaussRigidGen() {};
	RigidBody* generateParticles(double t) override;
	void init(Config& c) {
		RigidBodyGen::init(c);
		_dev_pos = c._dev_pos;
		_dev_vel = c._dev_vel;
	};

protected:
	Vector3 _dev_pos, _dev_vel;
	std::normal_distribution<float> dist;
	std::mt19937 random_generator;
};