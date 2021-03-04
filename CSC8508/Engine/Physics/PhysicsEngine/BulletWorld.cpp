#include "BulletWorld.h"
#include <algorithm>


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
	contactList.clear();
}

void BulletWorld::setGravity(NCL::Maths::Vector3 force)
{
	dynamicsWorld->setGravity(convertVector3(force));
}

GameObject* BulletWorld::rayIntersect(NCL::Maths::Vector3 from, NCL::Maths::Vector3 to)
{
	btVector3 btFrom = convertVector3(from);
	btVector3 btTo = convertVector3(to);
	btCollisionWorld::ClosestRayResultCallback res(btFrom, btTo);

	dynamicsWorld->rayTest(btFrom, btTo, res);
	if (res.hasHit())
	{
		return((GameObject*)res.m_collisionObject->getUserPointer());
	}

	return nullptr;
}

void BulletWorld::addRigidBody(RigidBody* body)
{
	dynamicsWorld->addRigidBody(body->returnBody());
	rigidList.push_back(body);
}

void BulletWorld::removeRigidBody(RigidBody* body)
{
	dynamicsWorld->removeRigidBody(body->returnBody());
	rigidList.erase(std::remove(rigidList.begin(), rigidList.end(), body), rigidList.end());
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
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();

		bool inList = false;
		collisionPair compare = std::make_pair(obA, obB);

		for (auto const& j : contactList)
			if (j == compare)
				inList = true;

		if (!inList)
		{
			contactList.push_back(compare);
			((GameObject*)obA->getUserPointer())->OnCollisionBegin(((GameObject*)obB->getUserPointer()));
			((GameObject*)obB->getUserPointer())->OnCollisionBegin(((GameObject*)obA->getUserPointer()));
		}
	}
	for (int j = 0; j < contactList.size(); j++)
	{
		bool inList = false;
		for (int i = 0; i < numManifolds; i++)
		{
			btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
			const btCollisionObject* obA = contactManifold->getBody0();
			const btCollisionObject* obB = contactManifold->getBody1();
			collisionPair compare = std::make_pair(obA, obB);

			if (contactList[j] == compare)
				inList = true;
		}
		if (!inList)
		{
			const btCollisionObject* obA = contactList[j].first;
			const btCollisionObject* obB = contactList[j].second;
			((GameObject*)obA->getUserPointer())->OnCollisionEnd(((GameObject*)obB->getUserPointer()));
			((GameObject*)obB->getUserPointer())->OnCollisionEnd(((GameObject*)obA->getUserPointer()));
			contactList.erase(contactList.begin() + j);
		}
	}

}