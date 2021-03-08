#include "RenderObject.h"
#include "../../Common/MeshGeometry.h"
#include "../../Common/MeshMaterial.h"

using namespace NCL::CSC8508;
using namespace NCL;

RenderObject::RenderObject(Transform* parentTransform, MeshGeometry* mesh, MeshMaterial* mat, TextureBase* tex, ShaderBase* shader) {
	this->transform	= parentTransform;
	this->mesh		= mesh;
	this->material	= mat;
	this->texture	= tex;
	this->shader	= shader;
	this->colour	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}

RenderObject::~RenderObject() {

}