#pragma once
#include "ActorObject.h"
#include "../../Common/Vector2.h"

namespace NCL {

	class Camera; 

	namespace CSC8508 {


		class PlayerObject : public ActorObject {

			friend class ObjectFactory;

		public:
			~PlayerObject();

			void ActorUpdate(float dt) override;
			void UpdateControls(Camera* camera);

		protected:
			PlayerObject(Game* game, Vector3 respawnPosition);


			bool lockOrientation;

			float yaw;
			float pitch;
			float cameraDistance;
		};
	}
}