#pragma once

#include <vector>
#include "Ray.h"
#include "CollisionDetection.h"
#include "QuadTree.h"
#include "GameObject.h"

namespace NCL {
		class Camera;
		using Maths::Ray;
	namespace CSC8508 {
		class GameObject;
		class Constraint;

		typedef std::function<void(GameObject*)> GameObjectFunc;
		typedef std::vector<GameObject*>::const_iterator GameObjectIterator;

		class GameWorld	{
		public:
			GameWorld();
			~GameWorld();

			void Clear();
			void ClearAndErase();

			std::vector<GameObject*> GetObjectsWithTag(std::string tag) const;
			GameObject* GetObjectWithTag(std::string tag) const;

			GameObject* AddGameObject(GameObject* o);
			void RemoveGameObject(GameObject* o, bool andDelete = false);

			void AddKillPlane(Plane* p);
			void RemoveKillPlane(Plane* p, bool andDelete = false);

			void AddConstraint(Constraint* c);
			void RemoveConstraint(Constraint* c, bool andDelete = false);

			QuadTree<GameObject*>* GetObjectTree() const {
				return objectTree;
			}

			QuadTree<GameObject*>* GetStaticObjectTree() const {
				return staticObjectTree;
			}

			void ShuffleConstraints(bool state) {
				shuffleConstraints = state;
			}

			void ShuffleObjects(bool state) {
				shuffleObjects = state;
			}

			bool Raycast(Ray& r, RayCollision& closestCollision, bool closestObject = false, bool includeStatic = false) const;

			std::vector<GameObject*> ObjectsWithinRadius(Vector3 position, float radius, std::string tag = "") const;

			virtual void UpdateWorld(float dt);

			void OperateOnContents(GameObjectFunc f);

			void GetObjectIterators(
				GameObjectIterator& first,
				GameObjectIterator& last) const;

			void GetConstraintIterators(
				std::vector<Constraint*>::const_iterator& first,
				std::vector<Constraint*>::const_iterator& last) const;

			void FlipDisplayQuadTree() { displayQuadtree = !displayQuadtree; }

			//This is very costly and should not be done regularly.
			template<class T>
			std::vector<GameObject*> GetObjectsWithComponent() const {
				static_assert(std::is_base_of<Component, T>::value, "Provided type is not a subclass of component");

				std::vector<GameObject*> objects;
				for (auto gameObject : gameObjects) {
					if (gameObject->GetComponent<T>()) {
						objects.push_back(gameObject);
					}
				}
				return objects;
			}

			template<class T>
			std::vector<T*> GetComponentsOfType() const {
				static_assert(std::is_base_of<Component, T>::value, "Provided type is not a subclass of component");

				std::vector<T*> components;
				for (auto gameObject : gameObjects) {
					T* component = gameObject->GetComponent<T>();
					if (component != nullptr)
						components.push_back(component);
				}
				return components;
			}

		protected:
			std::vector<GameObject*> newGameObjects;
			std::vector<GameObject*> gameObjects;
			std::vector<Constraint*> constraints;
			std::vector<Plane*>		 killPlanes;

			QuadTree<GameObject*>* objectTree;
			QuadTree<GameObject*>* staticObjectTree;

			bool	shuffleConstraints;
			bool	shuffleObjects;
			bool	displayQuadtree;
			int		worldIDCounter;
		};
	}
}