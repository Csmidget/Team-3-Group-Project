#include "OGLResourceManager.h"
#include "OGLMesh.h"
#include "OGLShader.h"
#include "OGLTexture.h"

using namespace NCL::Rendering;

OGLResourceManager::~OGLResourceManager() {
	for (auto m : loadedMeshes) {
		delete m.second;
	}
	loadedMeshes.clear();

	for (auto m : loadedShaders) {
		delete m.second;
	}
	loadedShaders.clear();

	for (auto m : loadedTextures) {
		delete m.second;
	}
	loadedTextures.clear();

	for (auto m : loadedCubeMaps) {
		delete m.second;
	}
	loadedCubeMaps.clear();
}

NCL::MeshGeometry* OGLResourceManager::LoadMesh(std::string fileName) {
	if (loadedMeshes.find(fileName) != loadedMeshes.end())
		return loadedMeshes[fileName];

	OGLMesh* mesh = new OGLMesh(fileName);
	mesh->SetPrimitiveType(GeometryPrimitive::Triangles);
	mesh->UploadToGPU();

	loadedMeshes.emplace(fileName, mesh);

	return mesh;
}

ShaderBase* OGLResourceManager::LoadShader(std::string shaderVert, std::string shaderFrag, std::string shaderGeom) {
	return nullptr;
}

TextureBase* OGLResourceManager::LoadTexture(std::string textureName, unsigned int flags, bool linearFilter, bool aniso) {
	return nullptr;
}

TextureBase* OGLResourceManager::LoadCubemap(std::string xPos, std::string xNeg, std::string yPos, std::string yNeg, std::string zPos, std::string zNeg, unsigned int flags) {
	return nullptr;
}