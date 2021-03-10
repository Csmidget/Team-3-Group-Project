



#include "../Engine/Component.h"
#include "../../Common/Vector3.h"
namespace NCL {
	namespace CSC8508 {

		class TeleportComponent :public Component
		{
		public:
			TeleportComponent(GameObject* object);
			void Start() override;
			//void OnTeleport() override;
			void OnCollisionBegin(GameObject* otherObject) override;

		private:
			Maths::Vector3 otherTeleportPosition;
		};

	}
}

