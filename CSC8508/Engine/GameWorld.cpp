#include "GameWorld.h"
#include "Constraint.h"
#include "CollisionDetection.h"
#include "../../Common/Camera.h"
#include <algorithm>
#include <vector>

using namespace NCL;
using namespace NCL::CSC8508;

GameWorld::GameWorld() {
	objectTree = new QuadTree<GameObject*>(Vector2(1024, 1024), 10, 6);
	staticObjectTree = new QuadTree<GameObject*>(Vector2(1024, 1024), 7, 6);
	shuffleConstraints	= false;
	shuffleObjects		= false;
	worldIDCounter		= 0;
	displayQuadtree = false;
}

GameWorld::~GameWorld()	{
	delete objectTree;
	delete staticObjectTree;
}

void GameWorld::Clear() {
	gameObjects.clear();
	newGameObjects.clear();
	constraints.clear();
	killPlanes.clear();

	staticObjectTree->Clear();
	objectTree->Clear();
}

void GameWorld::ClearAndErase() {
	for (int i = 0; i < gameObjects.size(); ++i) {
		delete gameObjects[i];
	}
	for (auto& i : constraints) {
		delete i;
	}
	for (auto& i : killPlanes) {
		delete i;
	}

	Clear();
}

std::vector<GameObject*> GameWorld::GetObjectsWithTag(std::string tag) const {
	std::vector<GameObject*> objectsWithTag;
	for (auto go : gameObjects)
	{
		if (go->HasTag(tag))
			objectsWithTag.push_back(go);
	}

	return objectsWithTag;

}

GameObject* NCL::CSC8508::GameWorld::GetObjectWithTag(std::string tag) const
{
	for (auto go : gameObjects)
	{
		if (go->HasTag(tag))
			return go;
	}
	return nullptr;
}

GameObject* GameWorld::AddGameObject(GameObject* o) {

	//For debugging, we should not be adding duplicate objects to the world.
	assert(std::find(gameObjects.begin(), gameObjects.end(), o) == gameObjects.end());

	gameObjects.emplace_back(o);
	newGameObjects.emplace_back(o);
	o->SetGameWorld(this);
	o->SetWorldID(worldIDCounter++);
	
	if (o->IsStatic()) {
		o->UpdateBroadphaseAABB();
		Vector3 halfSize;
		if (o->GetBroadphaseAABB(halfSize)) {
			staticObjectTree->Insert(o, o->GetTransform().GetPosition(), halfSize);
		}
	}

	return o;
}

void GameWorld::RemoveGameObject(GameObject* o, bool andDelete) {
	gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), o), gameObjects.end());
	if (andDelete) {
		delete o;
	}
}

void GameWorld::AddKillPlane(Plane* p) {
	killPlanes.emplace_back(p);
}

void GameWorld::RemoveKillPlane(Plane* p, bool andDelete) {
	killPlanes.erase(std::remove(killPlanes.begin(), killPlanes.end(), p), killPlanes.end());
	if (andDelete)
		delete p;
}

void GameWorld::GetObjectIterators(
	GameObjectIterator& first,
	GameObjectIterator& last) const {

	first	= gameObjects.begin();
	last	= gameObjects.end();
}

void GameWorld::OperateOnContents(GameObjectFunc f) {
	for (GameObject* g : gameObjects) {
		f(g);
	}
}

void GameWorld::UpdateWorld(float dt) {

	if (newGameObjects.size() > 0) {
		for (size_t i = 0; i < newGameObjects.size(); i++)
		{
			newGameObjects[i]->Start();
		}
		newGameObjects.clear();
	}

	objectTree->Clear();
	
	for (auto g : gameObjects) {
		if (!g->IsStatic() && g->IsActive()) {
			g->UpdateBroadphaseAABB();
			Vector3 gPos = g->GetTransform().GetPosition();
			for (auto p : killPlanes) {
				if (p->IsBehindPlane(gPos)) {
					std::cout << "Object \"" << g->GetName() << "\" is out of bounds.\n";
					g->OnKill();
				}
			}

			Vector3 halfSizes;
			if (!g->GetBroadphaseAABB(halfSizes)) {
				continue;
			}

			Vector3 pos = gPos;
			objectTree->Insert(g, pos, halfSizes);
		}	
	}

	//This must be done after generating object tree as some updates may want to test collisions
	for (auto g : gameObjects) {
		g->Update(dt);
	}

	if (shuffleObjects) {
		std::random_shuffle(gameObjects.begin(), gameObjects.end());
	}

	if (shuffleConstraints) {
		std::random_shuffle(constraints.begin(), constraints.end());
	}

	if (displayQuadtree)
		objectTree->DebugDraw();
}

bool GameWorld::Raycast(Ray& r, RayCollision& closestCollision, bool closestObject, bool includeStatic) const {
	RayCollision collision;

	std::set<GameObject*> possibleCollisions = objectTree->GetPossibleRayCollisions(r);


	if (includeStatic) {
		auto staticCollisions = staticObjectTree->GetPossibleRayCollisions(r);
		for (auto col : staticCollisions) {
			possibleCollisions.insert(col);
		}
	}

	for (auto& i : possibleCollisions) {
		if (!i->GetBoundingVolume() || i->GetCollisionLayer() & 1) { //objects might not be collideable etc...
			continue;
		}
		RayCollision thisCollision;
		if (CollisionDetection::RayIntersection(r, *i, thisCollision)) {

			if (!closestObject) {
				closestCollision = collision;
				closestCollision.node = i;
				return true;
			}
			else {
				if (thisCollision.rayDistance < collision.rayDistance) {
					thisCollision.node = i;
					collision = thisCollision;
				}
			}
		}
	}
	if (collision.node) {
		closestCollision = collision;
		closestCollision.node = collision.node;
		return true;
	}
	return false;
}


/*
Constraint Tutorial Stuff
*/

void GameWorld::AddConstraint(Constraint* c) {
	constraints.emplace_back(c);
}

void GameWorld::RemoveConstraint(Constraint* c, bool andDelete) {
	constraints.erase(std::remove(constraints.begin(), constraints.end(), c), constraints.end());
	if (andDelete) {
		delete c;
	}
}

void GameWorld::GetConstraintIterators(
	std::vector<Constraint*>::const_iterator& first,
	std::vector<Constraint*>::const_iterator& last) const {
	first	= constraints.begin();
	last	= constraints.end();
}



std::vector<GameObject*> GameWorld::ObjectsWithinRadius(Vector3 position, float radius, std::string tag) const {

	typedef std::pair<float, GameObject*> DistObjectPair;

	auto possibleObjects = objectTree->GetPossibleCollisions(position, Vector3(radius, radius, radius));
	
	//We do this to make the comparison operator cheaper (Otherwise it would have to find sqr distance for both
	//objects in every comparison.
	std::vector<DistObjectPair> foundObjects;

	float sqrRadius = radius * radius;
	for (auto object : possibleObjects) {

		//If a tag is defined. Only test objects with the tag.
		if (tag == "" || !object->HasTag(tag))
			continue;

		float sqrDist = (position - object->GetTransform().GetPosition()).LengthSquared();
		if (sqrDist <= sqrRadius)
			foundObjects.push_back({ sqrDist, object });
	}

	std::sort(foundObjects.begin(), foundObjects.end(), [&](DistObjectPair a, DistObjectPair b)->bool {
		return a.first < b.first;
	});

	std::vector<GameObject*> sortedObjects;

	for (auto o : foundObjects){
		sortedObjects.push_back(o.second);
	}

	return sortedObjects;
}
