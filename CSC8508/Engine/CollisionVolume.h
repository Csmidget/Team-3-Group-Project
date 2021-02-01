#pragma once
namespace NCL {
	enum class VolumeType {
		AABB	= 1,
		OBB		= 2,
		Sphere	= 4, 
		Mesh	= 8,
		Capsule = 16,
		Compound= 32,
		Invalid = 256
	};

	class CollisionVolume
	{
	public:
		CollisionVolume() {
			type = VolumeType::Invalid;
			active = true;
		}
		~CollisionVolume() {}

		bool IsActive() const { return active; }
		void SetActive(bool val) { active = val; }

		virtual void PrintDebugInfo(int& currLine, float lineSpacing) const = 0;

		VolumeType type;

	protected:
		bool active;
	};
}