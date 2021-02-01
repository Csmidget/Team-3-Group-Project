#pragma once

#include "../Engine/NavigationPath.h"

namespace NCL {
	namespace CSC8508 {

		enum class CameraState { PlayerMode = 0, FreeMode = 1, SelectionMode = 2 };
		
		class PlayerObject;
		class AIObject;
		class NavigationGrid;
		class Checkpoint;
		class ObjectFactory;
		class GameTechRenderer;
		class Checkpoint;
		class NavigationMap;
		class PhysicsSystem;
		class GameWorld;
		class PushdownMachine;

		class Game {
		public:
			Game();
			~Game();

			virtual void UpdateGame(float dt);


			bool GetUseGravity() const { return useGravity; }
			void SetUseGravity(bool val);

			bool ShouldQuit() const { return quit; }

			NavigationPath			GetPath(Vector3 start, Vector3 end,float maxCost = INFINITY) const;

			GameWorld*				GetWorld() const			{ return world; }
			PlayerObject*			GetPlayerObject() const		{ return player; }
			std::vector<AIObject*>	GetOpponents() const		{ return opponents; }
			const Checkpoint*		GetGoal() const				{ return goal; }
			const ObjectFactory*	GetObjectFactory() const	{ return objectFactory; }

			void InitPracticeWorld();
			void InitRaceWorld(int opponents);
			void InitKatamariWorld();
			void InitNavmeshWorld();

			void SetPause(bool val) { pause = val; }
			bool IsPaused() const { return pause; }

		protected:
			void Clear();

			void InitCamera();
			void UpdateKeys();
			void UpdateFramerateDisplay(float dt);

			void InitPracticeKillPlanes();
			void InitPracticeBaseGeometry();
			void InitPracticeGauntlet1();
			void InitPracticeSlope();
			void InitPracticeGauntlet2();
			void InitPracticePlayers();
			void InitPracticeCheckpoints();

			void InitRaceBaseGeometry();
			void InitRaceKillPlanes();
			void InitRacePlayers(int opponentCount);
			void InitRaceCheckpoints();

			void InitKatamariBaseGeometry();
			void InitKatamariKillPlanes();
			void InitKatamariPlayers(int opponentCount);

			void InitNavmeshLvlBaseGeometry();
			void InitNavmeshLvlKillPlanes();
			void InitNavmeshLvlPlayers(int opponentCount);

			ObjectFactory*	objectFactory;
			GameTechRenderer*	renderer;
			PhysicsSystem*		physics;
			GameWorld*			world;
			PushdownMachine*    gameStateMachine;

			std::vector<float> frameTimes;

			bool useGravity;
			bool pause;
			bool quit;

			PlayerObject* player;
			std::vector<AIObject*> opponents;
			std::vector<Checkpoint*> checkpoints;
			NavigationMap* navMap;
			Checkpoint* goal;
		};
	}
}

