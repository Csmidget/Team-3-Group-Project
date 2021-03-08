#pragma once
#include "GameTechRenderer.h"
#include "../Engine/PhysicsSystem.h"

namespace NCL {
	namespace CSC8508 {
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

			bool IntroSelectObject();
			GameObject* OpenCube = nullptr;
			GameObject* ExitCube = nullptr;
			GameObject* PauseCube = nullptr;
			GameObject* restartsqhere = nullptr;
			int OpenOrExit = 0;

			void InitGameExamples();

			void InitSlopeLevel();

			void InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			void InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims);
			void InitDefaultFloor();
			void BridgeConstraintTest();
			void DoorConstraintTest();
	
			bool SelectObject();
			void MoveSelectedObject();
			void DebugObjectMovement();
			void LockedObjectMovement();

			GameObject* AddFloorToWorld(const Vector3& position);
			GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass = 10.0f, bool respawning = false);
			GameObject* AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f, bool isStatic = false, bool respawning = false);
			GameObject* AddOBBCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f, bool isStatic = false, bool respawning = false);
			
			GameObject* AddCapsuleToWorld(const Vector3& position, float halfHeight, float radius, float inverseMass = 10.0f, bool respawning = false);

			GameObject* AddPlayerToWorld(const Vector3& position);
			GameObject* AddEnemyToWorld(const Vector3& position);
			GameObject* AddBonusToWorld(const Vector3& position);

			GameTechRenderer*	renderer;
			PhysicsSystem*		physics;
			GameWorld*			world;

			bool useGravity;
			bool inSelectionMode;

			float		forceMagnitude;

			GameObject* selectionObject = nullptr;
			GameObject* newselectionObject = nullptr;
			GameObject* forwardObject = nullptr;

			OGLMesh*	capsuleMesh = nullptr;
			OGLMesh*	cubeMesh	= nullptr;
			OGLMesh*	sphereMesh	= nullptr;
			OGLTexture* basicTex	= nullptr;
			OGLShader*	basicShader = nullptr;

			//Coursework Meshes
			OGLMesh*	charMeshA	= nullptr;
			OGLMesh*	charMeshB	= nullptr;
			OGLMesh*	enemyMesh	= nullptr;
			OGLMesh*	bonusMesh	= nullptr;

			//Coursework Additional functionality	
			GameObject* lockedObject	= nullptr;
			Vector3 lockedOffset		= Vector3(0, 14, 20);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}
			string name;
			Vector4 saveColor = Vector4(1, 1, 1, 1);
		};
	}
}

