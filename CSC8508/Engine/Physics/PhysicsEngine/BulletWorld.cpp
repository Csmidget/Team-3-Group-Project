#include "BulletWorld.h"

#include <algorithm>

#include "../../CSC8508/Engine/GameObject.h";

using namespace NCL;
using namespace CSC8508;
using namespace physics;

BulletWorld::BulletWorld()
{
	collisionConfiguration	=	new btDefaultCollisionConfiguration();
	dispatcher				=	new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache	=	new btDbvtBroadphase();
	solver					=	new btSequentialImpulseConstraintSolver;

	dynamicsWorld			=	new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

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

void BulletWorld::removeRigidBody(RigidBody* body)
{
	dynamicsWorld->removeRigidBody(body->returnBody());
	//rigidList.erase(std::remove(rigidList.begin(), rigidList.end(), body->returnBody()), rigidList.end());
	for (auto i : rigidList)
	{

	}
}

void BulletWorld::Update(float dt)
{
	dynamicsWorld->stepSimulation(1.f / 60.f);
	checkCollisions();

	for (auto i : rigidList)
	{
		i->updateTransform();
		i->returnBody()->applyDamping(1.f / 60.f);
	}
}

void BulletWorld::checkCollisions()
{
	int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
	std::cout << numManifolds << std::endl;;
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();

		if(contactManifold->getNumContacts() <= 0)
		{
			//((GameObject*)obA->getUserPointer())->OnCollisionEnd(((GameObject*)obB->getUserPointer()));
			//((GameObject*)obB->getUserPointer())->OnCollisionEnd(((GameObject*)obA->getUserPointer()));
		}


		//((GameObject*)obA->getUserPointer())->OnCollisionBegin(((GameObject*)obB->getUserPointer()));
		//((GameObject*)obB->getUserPointer())->OnCollisionBegin(((GameObject*)obA->getUserPointer()));
	}
}