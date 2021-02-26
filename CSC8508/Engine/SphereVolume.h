#pragma once
#include "CollisionVolume.h"

namespace NCL {
	class SphereVolume : CollisionVolume
	{
	public:
		SphereVolume(float sphereRadius = 1.0f) {
			type	= VolumeType::Sphere;
			radius	= sphereRadius;
		}
		~SphereVolume() {}

		float GetRadius() const {
			return radius;
		}

		void PrintDebugInfo(int& currLine, float lineSpacing) const override;

	protected:
		float	radius;
	};
}

