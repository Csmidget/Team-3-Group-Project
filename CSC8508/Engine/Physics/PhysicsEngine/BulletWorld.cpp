#include "BulletWorld.h"

using namespace NCL;
using namespace CSC8508;
using namespace physics;

BulletWorld::BulletWorld()
{
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	//sets gravity need to add member variable
	dynamicsWorld->setGravity(btVector3(0, -10, 0));
	
}

BulletWorld::~BulletWorld()
{
	delete dynamicsWorld;
	delete solver;
	delete overlappingPairCache;
	delete dispatcher;
	delete collisionConfiguration;

	rigidList.clear();
}

void BulletWorld::addRigidBody(RigidBody* body)
{
	dynamicsWorld->addRigidBody(body->returnBody());
	rigidList.push_back(body);
}

void BulletWorld::Update(float dt)
{
	dynamicsWorld->stepSimulation(1.f / 60.f);
	for (auto i : rigidList)
	{
		i->updateTransform();
		i->returnBody()->applyDamping(1.f / 60.f);
	}
}