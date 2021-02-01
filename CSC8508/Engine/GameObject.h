#pragma once
#include "Transform.h"
#include "CollisionVolume.h"

#include "PhysicsObject.h"
#include "RenderObject.h"

#include <vector>

using std::vector;

namespace NCL {
	namespace CSC8508 {

		class GameObject	{
		public:
			GameObject(string name = "");
			~GameObject();

			int GetCollisionLayer() {
				return collisionLayer;
			}
			void SetCollisionLayer(int val) {
				collisionLayer = val;
			}

			virtual void Update(float dt) {};

			void PrintDebugInfo() const;
			
			//Override to add debug info
			virtual void ObjectSpecificDebugInfo(int& currLine, float lineSpacing) const {};

			virtual void OnKill() { isActive = false; }

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

			bool IsStatic() const {
				return isStatic;
			}

			void SetIsStatic(bool val) {
				isStatic = val;
			}

			bool HasTag(std::string tag) {
				return std::find(tags.begin(), tags.end(), tag) != tags.end();			
			}

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

			virtual void OnCollisionBegin(GameObject* otherObject) {
				//std::cout << "OnCollisionBegin event occured!\n";
			}

			virtual void OnCollisionEnd(GameObject* otherObject) {
				//std::cout << "OnCollisionEnd event occured!\n";
			}

			bool GetBroadphaseAABB(Vector3&outsize) const;

			void UpdateBroadphaseAABB();

			void SetWorldID(int newID) {
				worldID = newID;
			}

			int		GetWorldID() const {
				return worldID;
			}

		protected:
			Transform			transform;

			CollisionVolume*	boundingVolume;
			PhysicsObject*		physicsObject;
			RenderObject*		renderObject;

			bool	isActive;
			bool	isStatic;
			int		worldID;
			int collisionLayer;
			string	name;
			Vector3 broadphaseAABB;
			std::vector<std::string> tags;
		};
	}
}

