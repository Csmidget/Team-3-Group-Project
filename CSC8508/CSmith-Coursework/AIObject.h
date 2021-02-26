#pragma once

#include "ActorObject.h"

#include "../Engine/NavigationPath.h"

#include "../../Common/Vector3.h"

namespace NCL {

	namespace CSC8508 {

		class BehaviourNode;

		class AIObject : public ActorObject {

			friend class ObjectFactory;

		public:
			~AIObject();

			void ActorUpdate(float dt) override;
			bool SetGoal(Vector3 newGoal, float maxCost = INFINITY, bool force = false);

			float GetCoinDetectRange() const { return cointDetectRange; }

			float GetCoinMaxDistance() const { return coinMaxDistance; }

			void ObjectSpecificDebugInfo(int& currLine, float lineSpacing) const override;

			void SetCurrentState(std::string str) { currentState = str; }

			void OnCollisionBegin(GameObject* otherObject) override;
			 
			bool IsAngry() { return currentAnger >= angerThreshold; }

			void OnRespawn() override;

			bool IsAsleep() const { return asleep; }
			void SetAsleep(bool val) { asleep = val; }

			void SetBehaviourTree(BehaviourNode* tree) { behaviourTree = tree; }

			void UpdateAngerColour();

		protected:
			AIObject(Game* game, Vector3 respawnPosition, std::string name = "enemy", float coinHuntRange = 10.0f, float coinMaxDistance = 30.0f, float angerThreshold = 3.0f, float strength = 2.0f);

			void DisplayPath() const;

			void UpdateMovement();
			void UpdateOrientation();

			bool asleep;

			float angerThreshold;
			float currentAnger;
			float strength;

			float behaviourUpdateCooldown;
			float cointDetectRange;
			float coinMaxDistance;
			std::string currentState;

			BehaviourNode* behaviourTree;
			Quaternion orientation;

			float initialNextNodeDistance;
			Vector3 nextNode;
			Vector3 currentGoal;
			NavigationPath currentPath;
		};
	}
}