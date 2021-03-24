#include "Transform.h"
#include "Debug.h"
#include "GameObject.h"
#include "PhysicsObject.h"

#include <iomanip>
#include <sstream>

using namespace NCL::CSC8508;

Transform::Transform(GameObject* object)
{
	this->gameObject = object;
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

Transform& Transform::SetPosition(const Vector3& worldPos, bool updatePhysics) {
	position = worldPos;

	if (updatePhysics && gameObject->GetPhysicsObject())
		gameObject->GetPhysicsObject()->body->setTransform();

	UpdateMatrix();
	return *this;
}

Transform& Transform::SetScale(const Vector3& worldScale) {
	scale = worldScale;
	UpdateMatrix();
	return *this;
}

Transform& Transform::SetOrientation(const Quaternion& worldOrientation, bool updatePhysics) {
	orientation = worldOrientation;

	if (updatePhysics && gameObject->GetPhysicsObject())
		gameObject->GetPhysicsObject()->body->setOrientation();

	UpdateMatrix();
	return *this;
}

std::vector<std::string> Transform::GetDebugInfo() const {
	std::vector<std::string> info;
	info.push_back("Transform");

	std::stringstream stream;
	stream << std::fixed << std::setprecision(2);

	stream << "  Position: "  << position.x << "," << position.y << "," << position.z;

	info.push_back(stream.str());
	stream.str("");

	stream << "  Orientation: " << orientation.x << "," << orientation.y << "," << orientation.z << "," << orientation.w;
	info.push_back(stream.str());
	stream.str("");

	stream << "  Scale: " << scale.x << "," << scale.y << "," << scale.z;
	info.push_back(stream.str());
	stream.str("");

	return info;
}