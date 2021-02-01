#include "CollisionDetection.h"
#include "CollisionVolume.h"
#include "AABBVolume.h"
#include "OBBVolume.h"
#include "SphereVolume.h"
#include "CapsuleVolume.h"

#include "Debug.h"

#include <sstream>
#include <iomanip>

using namespace NCL;

void AABBVolume::PrintDebugInfo(int& currLine, float lineSpacing) const {
	std::stringstream stream;

	stream << std::fixed << std::setprecision(2);

	stream << "Volume: " << "AABB";
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");

	stream << "Half Dims: " << halfSizes;
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");
}

void OBBVolume::PrintDebugInfo(int& currLine, float lineSpacing) const {
	std::stringstream stream;

	stream << std::fixed << std::setprecision(2);

	stream << "Volume: " << "OBB";
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");

	stream << "Half Dims: " << halfSizes;
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");
}

void SphereVolume::PrintDebugInfo(int& currLine, float lineSpacing) const {
	std::stringstream stream;

	stream << std::fixed << std::setprecision(2);

	stream << "Volume: " << "Sphere";
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");

	stream << "Radius: " << radius;
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");
}

void CapsuleVolume::PrintDebugInfo(int& currLine, float lineSpacing) const {
	std::stringstream stream;

	stream << std::fixed << std::setprecision(2);

	stream << "Volume: " << "Capsule";
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");

	stream << "Half Height: " << halfHeight;
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");

	stream << "Radius: " << radius;
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");

}