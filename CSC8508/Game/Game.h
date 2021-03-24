#pragma once
#include "../../Common/Vector3.h"
#include "../../Common/Vector4.h"

#include <string>

namespace NCL {

	namespace Rendering {
		class ResourceManager;
	}

	namespace CSC8508 {

		namespace Audio {
			class SoundInstance;
		}

		namespace physics {
			class BulletWorld;
		}

		class GameTechRenderer;
		class PhysicsSystem;
		class GameWorld;
		class GameObject;
		class PushdownMachine;
		class NetworkManager;

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
			bool IsAllPlayersFinished(); 
			bool IsMajorityPlayersFinished();
			bool IsNetworkGame() { return networkManager != nullptr; }
			
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
			GameObject* AddButtonToWorld(const Maths::Vector3& position, Maths::Vector3 dimensions, float inverseMass = 10.0f, bool isStatic = false);
			GameObject* AddCameraToWorld(const Maths::Vector3& position);

			GameObject* Raycast(const Maths::Vector3& from, const Maths::Vector3& to) const;

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
		
			std::string name;
			Maths::Vector4 saveColor = Maths::Vector4(1, 1, 1, 1);
		};
	}
}

