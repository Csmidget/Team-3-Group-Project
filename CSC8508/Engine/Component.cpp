#include "Component.h"
#include "GameObject.h"

using namespace NCL::CSC8508;

std::vector<std::string> Component::GetDebugInfo() {
	std::vector<std::string> info;
	info.push_back(name);
	
	auto subclassInfo = DebugInfo();
	info.insert(info.end(), subclassInfo.begin(),subclassInfo.end());
	return info;
}

Component::Component(std::string name, GameObject* object) {
	gameObject = object;
	transform = &object->GetTransform();
	enabled = true;
	this->name = name;
}