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

	if (animation)
		animRelativeJoints = anim->GenerateRelativeJoints(mesh->GetInverseBindPose());
}

RenderObject::~RenderObject() {

}