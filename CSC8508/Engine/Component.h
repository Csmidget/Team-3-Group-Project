#pragma once
#include <vector>
#include <string>

namespace NCL {
	namespace CSC8508 {

		class GameObject;
		class Transform;

		class Component {
		public:

			Component(std::string componentName, GameObject* object);
			virtual ~Component() {};
			virtual void Start() {};
			virtual void Update(float dt) {};
			virtual void fixedUpdate(float dt) {};
			virtual void OnCollisionBegin(GameObject* otherObject) {};
			virtual void OnCollisionStay(GameObject* otherObject) {};
			virtual void OnCollisionEnd(GameObject* otherObject) {};
			virtual void OnKill() {};
			std::vector<std::string> GetDebugInfo();

			bool IsEnabled() const		{ return enabled; }
			void SetEnabled(bool val)	{ enabled = val; }

			std::string GetName() { return name; }

		protected:
			virtual std::vector<std::string> DebugInfo() { return std::vector<std::string>(); }

			GameObject* gameObject;
			Transform* transform;

		private:
			bool enabled;
			std::string name;
		};

	}
}