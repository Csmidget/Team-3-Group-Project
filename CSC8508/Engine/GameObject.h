#pragma once
#include "Transform.h"
#include "CollisionVolume.h"
#include "PhysicsObject.h"
#include "RenderObject.h"

#include <unordered_set>
#include <algorithm>

using std::vector;

namespace NCL {
	namespace CSC8508 {

		class Component;
		class GameWorld;

		class GameObject	{

			friend class GameWorld;

		public:
			GameObject(string name = "");
			~GameObject();

			int GetCollisionLayer() {
				return collisionLayer;
			}
			void SetCollisionLayer(int val) {
				collisionLayer = val;
			}

			void Update(float dt);

			virtual void OnUpdate(float dt) {};

			void fixedUpdate(float dt);

			void Destroy() { destroy = true; }
			
			//Override to add debug info
			virtual void ObjectSpecificDebugInfo(int& currLine, float lineSpacing) const {};

			virtual void OnKill();

			void SetBoundingVolume(CollisionVolume* vol) {
				boundingVolume = vol;
			}

			const CollisionVolume* GetBoundingVolume() const {
				return boundingVolume;
			}

			bool IsActive() const {
				return isActive;
			}

			void SetIsActive(bool val) {
				isActive = val;
			}

			bool IsPersistent() const {
				return persistent;
			}

			void SetPersistence(bool val) {
				persistent = val;
			}

			bool IsStatic() const {
				return isStatic;
			}

			void SetIsStatic(bool val) {
				isStatic = val;
			}

			void AddTag(std::string tag) {
				tags.insert(tag);
			}

			bool HasTag(std::string tag) {
				return tags.find(tag) != tags.end();			
			}

			GameWorld* GetWorld() { return world; }

			Transform& GetTransform() {
				return transform;
			}

			const Transform& GetTransform() const {
				return transform;
			}

			RenderObject* GetRenderObject() const {
				return renderObject;
			}

			PhysicsObject* GetPhysicsObject() const {
				return physicsObject;
			}

			void SetRenderObject(RenderObject* newObject) {
				renderObject = newObject;
			}

			void SetPhysicsObject(PhysicsObject* newObject) {
				physicsObject = newObject;
			}

			const string& GetName() const {
				return name;
			}

			std::vector<std::string> DebugInfo() const;

			void OnCollisionBegin(GameObject* otherObject);
			void OnCollisionStay(GameObject* otherObject);
			void OnCollisionEnd(GameObject* otherObject);
		

			bool GetBroadphaseAABB(Vector3&outsize) const;

			void UpdateBroadphaseAABB();

			void SetWorldID(int newID) {
				worldID = newID;
			}

			int	GetWorldID() const {
				return worldID;
			}

			template<typename T, typename... Params>
			T* AddComponent(Params... vals) {
				T* component = new T(this, vals...);
				components.push_back(component);
				return component;
			}

			template<typename T>
			T* GetComponent() const {
				for (auto component : components) {
					T* t = dynamic_cast<T*>(component);
		
					if (t != nullptr) {
						return t;
					}
				}
		
				return nullptr;
			}

			template<typename T>
			void RemoveComponent() {
				for (int i = components.size() - 1; i >= 0; --i) {
					if (dynamic_cast<T*>(components[i]) != nullptr) {
						delete components[i];
						components.erase(components.begin() + i);
					}
				}
			}

		protected:

			void Start();
			void SetGameWorld(GameWorld* world);

			Transform			transform;

			CollisionVolume*	boundingVolume;
			PhysicsObject*		physicsObject;
			RenderObject*		renderObject;
			GameWorld*			world;

			bool	isActive;
			bool	isStatic;
			bool	persistent;
			bool	destroy;

			int		worldID;
			int collisionLayer;
			string	name;
			Vector3 broadphaseAABB;
			std::unordered_set<std::string> tags;
			std::vector<Component*> components;
		};
	}
}

