#include "BulletWorld.h"
#include <algorithm>


using namespace NCL;
using namespace CSC8508;
using namespace physics;


BulletWorld::BulletWorld()
{
	//creates the bulletworld wwith default parameters
	collisionConfiguration	=	new btDefaultCollisionConfiguration();
	dispatcher				=	new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache	=	new btDbvtBroadphase();
	solver					=	new btSequentialImpulseConstraintSolver;
	dynamicsWorld			=	new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -20.0f, 0));
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

//sets gravity
void BulletWorld::setGravity(NCL::Maths::Vector3 force)
{
	dynamicsWorld->setGravity(convertVector3(force));
}

void BulletWorld::addhingeconstraint(RigidBody* bodyA){
	//GameObject* B = AddCubeToWorld(Vector3(0, 20, 0), Vector3(1, 1, 1), 10);
	//GameObject* bodyA = AddCubeToWorld(Vector3(0, 30, 0), Vector3(1, 1, 1), 10);
	//physics->addhingeconstraint(A->GetPhysicsObject()->body, B->GetPhysicsObject()->body);
	btVector3 pivot(1, 1, 1);
	btTypedConstraint* p2p = new btPoint2PointConstraint(*bodyA ->returnBody(), pivot);
	m_dynamicsWorld->addConstraint(p2p);
	p2p->setBreakingImpulseThreshold(10.2);
	p2p->setDbgDrawSize(btScalar(5.f));
	//btVector3 pivot(1, 1, 1);
	//btRigidBody* body0 = createRigidBody(1, 1, 1);	//rbA = AddCubeToWorld(Vector3(0, 10, 0), Vector3(1, 1, 1));
	//btPoint2PointConstraint(RigidBody & B, const btVector3 & pivotInA);
	//btPoint2PointConstraint(RigidBody & A, btRigidBody & B, const btVector3 & pivotInA, const btVector3 & pivotInB);
	//btTransform trans;
	//float mass = 1.f;
	//btCollisionShape* shape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
	//btRigidBody* createRigidBody(float mass, const btTransform & startTransform, btCollisionShape * shape, const btVector4 & color = btVector4(1, 0, 0, 1));
	//AddCubeToWorld(Vector3(0, 10, 0), Vector3(1, 1, 1), 0.0f);
	//btRigidBody* AddRigidBody;
	// trans.setIdentity();
	// trans.setOrigin(btVector3(1, 30, -5));
	 //AddRigidBody(mass, trans, shape);
	// trans.setOrigin(btVector3(0, 0, -5));
	 //btRigidBody* body0 = createRigidBody(mass, trans, shape);
	// trans.setOrigin(btVector3(2 * CUBE_HALF_EXTENTS, 20, 0));
	 //mass = 1.f;
	 //	btRigidBody* body1 = 0;//createRigidBody( mass,trans,shape);
	// btVector3 pivotInA(CUBE_HALF_EXTENTS, CUBE_HALF_EXTENTS, 0);
	 //btTypedConstraint* p2p = new btPoint2PointConstraint(*body0, pivotInA);
	// m_dynamicsWorld->addConstraint(p2p);
	// p2p->setBreakingImpulseThreshold(10.2);
	 //p2p->setDbgDrawSize(btScalar(5.f));
	 

}

//cast a ray between to points and returns the hit gameobject or nullptr if nothing is hit
GameObject* BulletWorld::rayIntersect(	NCL::Maths::Vector3 from, NCL::Maths::Vector3 to,
								/*OUT*/ NCL::Maths::Vector3 pointHit)
{
	btVector3 btFrom = convertVector3(from);
	btVector3 btTo = convertVector3(to);
	btCollisionWorld::ClosestRayResultCallback res(btFrom, btTo);
	dynamicsWorld->rayTest(btFrom, btTo, res);
	if (res.hasHit())
	{
		pointHit = convertbtVector3(res.m_hitPointWorld);
		return((GameObject*)res.m_collisionObject->getUserPointer());
	}
	return nullptr;
}

//adds a rigidbody to the simulation
void BulletWorld::addRigidBody(RigidBody* body)
{
	dynamicsWorld->addRigidBody(body->returnBody());
	rigidList.push_back(body);
}

//removes rigid body must be called before deleting a game object to remove the associatd body from the sim
void BulletWorld::removeRigidBody(RigidBody* body)
{
	dynamicsWorld->removeRigidBody(body->returnBody());
	rigidList.erase(std::remove(rigidList.begin(), rigidList.end(), body), rigidList.end());
}

//steps simulation and sets the transform based on bullet physics
void BulletWorld::Update(float dt)
{
	dynamicsWorld->stepSimulation(dt ,10);
	checkCollisions();

	for (auto i : rigidList)
	{
		i->updateTransform();
		i->returnBody()->applyDamping(dt /*1.f / 60.f*/);
	}
}

//checks all manifolds for new and expired manifolds to activate the Oncollision end and begin functions
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

		((GameObject*)obA->getUserPointer())->OnCollisionStay(((GameObject*)obB->getUserPointer()));
		((GameObject*)obB->getUserPointer())->OnCollisionStay(((GameObject*)obA->getUserPointer()));
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

//remove rigidbodies form the dynamics world and clear our lists so that we can repopulate the world
//might need to create the dynamics world again need to test properly. Alternativly meta data in the 
//dispatcher etc may need to be reset manually here
void BulletWorld::clear()
{
	for (auto  i: rigidList)
	{
		removeRigidBody(i);
	}
	rigidList.clear();
	contactList.clear();
}