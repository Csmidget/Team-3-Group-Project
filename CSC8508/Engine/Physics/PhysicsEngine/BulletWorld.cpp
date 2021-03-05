#include "BulletWorld.h"
//#include "RigidBody.h"
//#include "../../../Game/Game.h"
//#include "../../../Game/Game.cpp"

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

void BulletWorld::addhingeconstraint(RigidBody* bodyA, RigidBody* bodyB){
	//GameObject* B = AddCubeToWorld(Vector3(0, 20, 0), Vector3(1, 1, 1), 10);
	//GameObject* A = AddCubeToWorld(Vector3(0, 30, 0), Vector3(1, 1, 1), 10);
	//physics->addhingeconstraint(A->GetPhysicsObject()->body, B->GetPhysicsObject()->body);
	
	//rbA = AddCubeToWorld(Vector3(0, 10, 0), Vector3(1, 1, 1));
	//btPoint2PointConstraint(btRigidBody & B, const btVector3 & pivotInA);
	//btPoint2PointConstraint(btRigidBody & A, btRigidBody & B, const btVector3 & pivotInA, const btVector3 & pivotInB);
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