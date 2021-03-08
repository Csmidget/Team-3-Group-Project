#pragma once
#include "../../Common/ResourceManager.h"
#include "../../Common/Vector3.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include <string>
#include "../Engine/Physics/PhysicsEngine/BulletWorld.h"
#include "../Engine/NetworkManager.h"
#include "GameStateManager.h"

namespace NCL {
	namespace CSC8508 {

		class GameTechRenderer;
		class PhysicsSystem;
		class GameWorld;
		class GameObject;

		class Game		{
		public:
			Game(string name = "");
			~Game();

			

			virtual void UpdateGame(float dt);
			virtual void UpdateIntroGame(float dt);
			virtual void UpdatePauseGame(float dt);

			int GetOpenOrExit() {
				return OpenOrExit;
			}
			void SetOpenOrExit(int openorexit) {
				OpenOrExit = openorexit;
			}

			void AddGameObject(GameObject* go);

			GameWorld* GetWorld() const { return world; }
			physics::BulletWorld* GetPhysics() const { return physics; }

			NCL::Rendering::ResourceManager* GetResourceManager() { return resourceManager; }

		protected:

			void Clear();

			void InitialiseAssets();

			void InitCamera();
			void UpdateKeys();

			void InitWorld();
			void InitIntroWorld();
			void InitOverWorld();
			void InitPauseWorld();
			void InitIntroCamera();
			void InitOpenCube();
			void InitExitCube();
			
			void InitFromJSON(std::string fileName);

			void InitGameExamples();

			void InitSlopeLevel();
			GameObject* OpenCube = nullptr;
			GameObject* ExitCube = nullptr;
			GameObject* PauseCube = nullptr;
			GameObject* restartsqhere = nullptr;
			int OpenOrExit = 0;
			
			void InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			void InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Maths::Vector3& cubeDims);
			void InitDefaultFloor();
			void BridgeConstraintTest();
			void DoorConstraintTest();
	
			bool SelectObject();
			bool IntroSelectObject();
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
			GameObject* AddButtonToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f, bool isStatic = false, bool respawning = false);


			GameTechRenderer*	renderer;
			//PhysicsSystem*		physics;
			GameWorld*			world;
			NCL::Rendering::ResourceManager* resourceManager;
			physics::BulletWorld* physics;

			NetworkManager* networkManager;
			GameStateManager* gameStateManager;

			bool useGravity;
			bool inSelectionMode;

			float		forceMagnitude;

			GameObject* selectionObject = nullptr;
			GameObject* newselectionObject = nullptr;
			GameObject* forwardObject = nullptr;

			// OGLShader * m_temp_shader = nullptr;
			//GameObject* m_tempp;

			

			//Coursework Additional functionality	
			GameObject* lockedObject	= nullptr;
			Maths::Vector3 lockedOffset		= Maths::Vector3(0, 14, 20);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}
			string name;
			Vector4 saveColor = Vector4(1, 1, 1, 1);
		};
	}
}

