#include "Transform.h"
#include <iomanip>
#include <sstream>
#include "Debug.h"

using namespace NCL::CSC8508;

Transform::Transform()
{
	scale	= Vector3(1, 1, 1);
}

Transform::~Transform()
{

}

void Transform::UpdateMatrix() {
	matrix =
		Matrix4::Translation(position) *
		Matrix4(orientation) *
		Matrix4::Scale(scale);
}

Transform& Transform::SetPosition(const Vector3& worldPos) {
	position = worldPos;
	UpdateMatrix();
	return *this;
}

Transform& Transform::SetScale(const Vector3& worldScale) {
	scale = worldScale;
	UpdateMatrix();
	return *this;
}

Transform& Transform::SetOrientation(const Quaternion& worldOrientation) {
	orientation = worldOrientation;
	UpdateMatrix();
	return *this;
}

void Transform::PrintDebugInfo(int& currLine, float lineSpacing) const {
	std::stringstream stream;

	stream << std::fixed << std::setprecision(2);

	stream << "Position: "  << position;
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");

	stream << "Orientation: " << orientation;
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");

	stream << "Scale: " << scale;
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");
}