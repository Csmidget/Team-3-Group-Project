#pragma once
#include "../../Common/Matrix4.h"
#include "../../Common/Matrix3.h"
#include "../../Common/Vector3.h"
#include "../../Common/Quaternion.h"

#include <vector>

using std::vector;

using namespace NCL::Maths;

namespace NCL {
	namespace CSC8508 {
		class GameObject;

		class Transform
		{
		public:
			Transform(GameObject* object);
			~Transform();

			Transform& SetPosition(const Vector3& worldPos, bool updatePhysics = true);
			Transform& SetScale(const Vector3& worldScale);
			Transform& SetOrientation(const Quaternion& newOr, bool updatePhysics = true);

			Vector3 GetPosition() const {
				return position;
			}

			Vector3 GetScale() const {
				return scale;
			}

			Quaternion GetOrientation() const {
				return orientation;
			}

			Matrix4 GetMatrix() const {
				return matrix;
			}
			void UpdateMatrix();

			std::vector<std::string> GetDebugInfo() const;

		protected:
			GameObject* gameObject;
			Matrix4		matrix;
			Quaternion	orientation;
			Vector3		position;

			Vector3		scale;
		};
	}
}

