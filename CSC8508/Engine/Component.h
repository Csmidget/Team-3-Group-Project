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

		protected:
			GameObject* gameObject;
			Transform* transform;
		};

	}
}