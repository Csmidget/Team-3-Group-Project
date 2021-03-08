#include "OGLResourceManager.h"
#include "OGLMesh.h"
#include "OGLShader.h"
#include "OGLTexture.h"

#include "../../Common/MeshMaterial.h"
#include "../../Common/TextureLoader.h"

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

	for (auto m : loadedMaterials) {
		delete m.second;
	}
	loadedMaterials.clear();

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

NCL::MeshMaterial* OGLResourceManager::LoadMaterial(std::string fileName) {
	 
	if (loadedMaterials.find(fileName) != loadedMaterials.end())
		return loadedMaterials[fileName];

	MeshMaterial* material = new MeshMaterial(fileName);

	material->LoadTextures(this);

	loadedMaterials.emplace(fileName, material);

	return material;
}

ShaderBase* OGLResourceManager::LoadShader(std::string shaderVert, std::string shaderFrag, std::string shaderGeom) {

	std::string combinedName = shaderVert + shaderFrag + shaderGeom;

	if (loadedShaders.find(combinedName) != loadedShaders.end())
		return loadedShaders[combinedName];

	ShaderBase* shader = new OGLShader(shaderVert, shaderFrag, shaderGeom);

	loadedShaders.emplace(combinedName, shader);

	return shader;
}

TextureBase* OGLResourceManager::LoadTexture(std::string textureName, unsigned int flags, bool linearFilter, bool aniso) {

	//If we have loaded this texture in the past, just return it again.
	if (loadedTextures.find(textureName) != loadedTextures.end())
		return loadedTextures[textureName];

	TextureBase* tex = TextureLoader::LoadAPITexture(textureName);

	loadedTextures.emplace(textureName, tex);

	return tex;
}

TextureBase* OGLResourceManager::LoadCubemap(std::string xPos, std::string xNeg, std::string yPos, std::string yNeg, std::string zPos, std::string zNeg, unsigned int flags) {
	return nullptr;
}