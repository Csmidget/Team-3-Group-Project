#pragma once
#include "../../Common/ResourceManager.h"
#include "../../Common/Vector3.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include <string>
#include <queue>

#include "../Engine/Physics/PhysicsEngine/BulletWorld.h"
#include "../Engine/NetworkManager.h"
#include"../Audio/SoundInstance.h"

namespace NCL {
	namespace CSC8508 {

		class GameTechRenderer;
		class PhysicsSystem;
		class GameWorld;
		class GameObject;
		class PushdownMachine;

		class Game		{
		public:
			Game();
			~Game();

			void InitWorld(std::string levelName, bool forceClear = false);
			void InitIntroWorld();
			void InitNetworkPlayers();
			bool IsExitLobbyTime();

			void EnableNetworking(bool client);
			void DisableNetworking();
			
			virtual bool UpdateGame(float dt);

			void SetPaused(bool pause) {
				paused = pause;
			}

			void AddGameObject(GameObject* go);
			GameObject* AddFloorToWorld(const Maths::Vector3& position);
			GameObject* AddSphereToWorld(const Maths::Vector3& position, float radius, float inverseMass = 10.0f);
			GameObject* AddCubeToWorld(const Maths::Vector3& position, Maths::Vector3 dimensions, float inverseMass = 10.0f, bool isStatic = false);
			GameObject* AddOBBCubeToWorld(const Maths::Vector3& position, Maths::Vector3 dimensions, float inverseMass = 10.0f, bool isStatic = false);
			GameObject* AddCapsuleToWorld(const Maths::Vector3& position, float halfHeight, float radius, float inverseMass = 10.0f);
			GameObject* AddPlayerToWorld(const Maths::Vector3& position);
			GameObject* AddEnemyToWorld(const Maths::Vector3& position);
			GameObject* AddBonusToWorld(const Maths::Vector3& position);
			GameObject* AddButtonToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f, bool isStatic = false);
			GameObject* AddCameraToWorld(const Vector3& position);

			GameObject* Raycast(const Vector3& from, const Vector3& to) const;

			GameWorld* GetWorld() const { return world; }
			physics::BulletWorld* GetPhysics() const { return physics; }

			GameTechRenderer* getRenderer() { return renderer; }

			NCL::Rendering::ResourceManager* GetResourceManager() { return resourceManager; }

		protected:

			void InitIntroCamera();

			void Clear(bool force);

			void InitialiseAssets();
			void InitDefaultFloor();

			void InitCamera();
			void UpdateKeys();
						
			void InitFromJSON(std::string fileName);


			GameTechRenderer*	renderer;
			GameWorld*			world;
			NCL::Rendering::ResourceManager* resourceManager;
			physics::BulletWorld* physics;
			PushdownMachine* gameStateMachine;
			NetworkManager* networkManager;
			Audio::SoundInstance* music;

			bool useGravity;
			bool inSelectionMode;
			bool paused;

			float	forceMagnitude;
		
			string name;
			Vector4 saveColor = Vector4(1, 1, 1, 1);
		};
	}
}

