#include "RenderObject.h"
#include "../../Common/MeshGeometry.h"
#include "../../Common/MeshMaterial.h"
#include "../../Common/MeshAnimation.h"

using namespace NCL::CSC8508;
using namespace NCL;

RenderObject::RenderObject(Transform* parentTransform, MeshGeometry* mesh, MeshMaterial* mat, TextureBase* tex, MeshAnimation* anim, ShaderBase* shader) {
	this->transform	= parentTransform;
	this->mesh		= mesh;
	this->material	= mat;
	this->animation = anim;
	this->texture	= tex;
	this->shader	= shader;
	this->colour	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	if (anim)
		animRelativeJoints = anim->GenerateRelativeJoints(mesh->GetInverseBindPose());
}

void RenderObject::SetAnimation(MeshAnimation* anim) {
	animation = anim;
	currentFrame = 0;

	if (animation) {
		animRelativeJoints = anim->GenerateRelativeJoints(mesh->GetInverseBindPose());
		frameTime = 1.0f / animation->GetFrameRate();
	}
}

void RenderObject::Update(float dt) {
	if (animation) {

		frameTime -= dt;
		while (frameTime < 0.0f) {
			currentFrame = (currentFrame + 1) % animation->GetFrameCount();
			frameTime += 1.0f / animation->GetFrameRate();
		}

		frameMatrices.clear();
		const Matrix4* prevFrameData = GetRelativeJointData(currentFrame == 0 ? animation->GetFrameCount() - 1 : currentFrame - 1);
		const Matrix4* currFrameData = GetRelativeJointData(currentFrame);

		float progress = frameTime / (1.0f / animation->GetFrameRate());
		for (unsigned int j = 0; j < mesh->GetJointCount(); j++) {
			frameMatrices.push_back(Matrix4::LerpTransforms(progress, currFrameData[j], prevFrameData[j]));
		}
	}
}

const Matrix4* RenderObject::GetRelativeJointData(unsigned int frame) const {

	if (frame >= animation->GetFrameCount()) {
		return nullptr;
	}
	int matStart = frame * animation->GetJointCount();

	Matrix4* dataStart = (Matrix4*)animRelativeJoints.data();

	return dataStart + matStart;
}

RenderObject::~RenderObject() {

}