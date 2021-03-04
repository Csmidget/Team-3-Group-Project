#pragma once
#include "../../Common/ResourceManager.h"
#include "../../Common/Vector3.h"

#include <string>

namespace NCL {
	namespace CSC8508 {

		class GameTechRenderer;
		class PhysicsSystem;
		class GameWorld;
		class GameObject;

		class Game {
		public:
			Game();
			~Game();

			virtual void UpdateGame(float dt);

			void AddGameObject(GameObject* go);

			GameWorld* GetWorld() const { return world; }
			NCL::Rendering::ResourceManager* GetResourceManager() { return resourceManager; }

		protected:

			void Clear();

			void InitialiseAssets();

			void InitCamera();
			void UpdateKeys();

			void InitWorld();

			void InitFromJSON(std::string fileName);

			void InitGameExamples();

			void InitSlopeLevel();
			void InitLight();
			void InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			void InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Maths::Vector3& cubeDims);
			void InitDefaultFloor();
			void BridgeConstraintTest();
			void DoorConstraintTest();

			bool SelectObject();
			void MoveSelectedObject();
			void DebugObjectMovement();
			void LockedObjectMovement();

			GameObject* AddFloorToWorld(const Maths::Vector3& position);
			GameObject* AddSphereToWorld(const Maths::Vector3& position, float radius, float inverseMass = 10.0f, bool respawning = false);
			GameObject* AddCubeToWorld(const Maths::Vector3& position, Maths::Vector3 dimensions, float inverseMass = 10.0f, bool isStatic = false, bool respawning = false);
			GameObject* AddOBBCubeToWorld(const Maths::Vector3& position, Maths::Vector3 dimensions, float inverseMass = 10.0f, bool isStatic = false, bool respawning = false);

			GameObject* AddCapsuleToWorld(const Maths::Vector3& position, float halfHeight, float radius, float inverseMass = 10.0f, bool respawning = false);

			GameObject* AddPlayerToWorld(const Maths::Vector3& position);
			GameObject* AddEnemyToWorld(const Maths::Vector3& position);
			GameObject* AddBonusToWorld(const Maths::Vector3& position);

			GameTechRenderer* renderer;
			PhysicsSystem* physics;
			GameWorld* world;
			NCL::Rendering::ResourceManager* resourceManager;

			bool useGravity;
			bool inSelectionMode;

			float		forceMagnitude;

			GameObject* selectionObject = nullptr;
			GameObject* forwardObject = nullptr;

			//Coursework Additional functionality	
			GameObject* lockedObject = nullptr;
			Maths::Vector3 lockedOffset = Maths::Vector3(0, 14, 20);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}

		};
	}
}

