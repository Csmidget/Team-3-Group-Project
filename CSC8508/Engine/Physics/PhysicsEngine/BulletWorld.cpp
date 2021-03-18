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
	dynamicsWorld->setInternalTickCallback((btInternalTickCallback)tickCallBack, this, true);
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

void BulletWorld::addpointconstraint(RigidBody* bodyA, NCL::Maths::Vector3 point){

	//btVector3 pivot(1, 5, 1);// = convertVector3(point);
	btVector3 pivot = convertVector3(point);
	btTypedConstraint* p2p = new btPoint2PointConstraint(*bodyA ->returnBody(), pivot);
	p2p->setBreakingImpulseThreshold((btScalar)10000.2);
	dynamicsWorld->addConstraint(p2p);
}

void BulletWorld::addhingeconstraint(RigidBody* doorbody, NCL::Maths::Vector3 point, NCL::Maths::Vector3 axisA) {

	btHingeConstraint* doorhinge = NULL;
	//const btVector3 pivot(1.0f, 2.0f, 1.0f);
	const btVector3 pivot = convertVector3(point);
	//btVector3 axis(0.0f, 1.0f, 0.0f);
	btVector3 axis = convertVector3(axisA);
	doorhinge = new btHingeConstraint(*doorbody->returnBody(), pivot, axis);
	//doorhinge->setLimit(-3.1415 * 0.25f, 3.1415 * 0.25f);
	dynamicsWorld->addConstraint(doorhinge);
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

void BulletWorld::addUpdateObject(GameObject* object)
{
	updateList.push_back(object);
}

void BulletWorld::removeUpdateObject(GameObject* object)
{
	updateList.erase(std::remove(updateList.begin(), updateList.end(), object), updateList.end());
}

//steps simulation and sets the transform based on bullet physics
void BulletWorld::Update(float dt)
{
	dynamicsWorld->stepSimulation((btScalar)dt, 4);
	checkCollisions();

	for (auto i : rigidList)
	{
		i->returnBody()->applyDamping((btScalar)dt);
		i->returnBody()->integrateVelocities((btScalar)dt);
		i->updateTransform();
	}
	
}

//checks all manifolds for new and expired manifolds to activate the Oncollision end and begin functions
void BulletWorld::checkCollisions()
{
	int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		//std::cout << contactManifold->getNumContacts() << std::endl;
		if (contactManifold->getNumContacts() > 0)
		{
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
	updateList.clear();
}


//callback tests
void BulletWorld::tickCallBack(btDynamicsWorld* world, btScalar timeStep)
{
	BulletWorld* worldRef = (BulletWorld*)world->getWorldUserInfo();
	worldRef->updateObjects(timeStep);
}

void BulletWorld::updateObjects(float dt)
{
	for (auto i : rigidList)
	{
		((GameObject*)i->returnBody()->getUserPointer())->fixedUpdate(dt);
	}
}