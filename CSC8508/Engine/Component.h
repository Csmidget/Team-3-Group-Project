#pragma once

namespace NCL {
	namespace CSC8508 {

		class GameObject;
		class Transform;

		class Component {

		public:

			Component(GameObject* object);
			virtual void Start() {};
			virtual void Update(float dt) {};
			virtual void OnCollisionBegin(GameObject* otherObject) {};
			virtual void OnCollisionStay(GameObject* otherObject) {};
			virtual void OnCollisionEnd(GameObject* otherObject) {};
			virtual void OnKill() {};

		protected:
			GameObject* gameObject;
			Transform* transform;
		};

	}
}