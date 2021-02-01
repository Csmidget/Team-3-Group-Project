#pragma once
#include "CollisionVolume.h"
#include "../../Common/Vector3.h"

namespace NCL {
	class AABBVolume : public CollisionVolume
	{
	public:
		AABBVolume(const Vector3& halfDims) {
			type		= VolumeType::AABB;
			halfSizes	= halfDims;
		}
		~AABBVolume() {

		}

		Vector3 GetHalfDimensions() const {
			return halfSizes;
		}

		void PrintDebugInfo(int& currLine, float lineSpacing) const override;


	protected:
		Vector3 halfSizes;
	};
}
