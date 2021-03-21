#pragma once

#include "../../Common/ResourceManager.h"

namespace NCL {

	namespace Rendering {

		class OGLResourceManager : public ResourceManager {

		public:

			~OGLResourceManager();

			NCL::MeshGeometry*	LoadMesh(std::string fileName) override;
			ShaderBase*			LoadShader(std::string shaderVert, std::string shaderFrag, std::string shaderGeom = "") override;
			TextureBase*		LoadTexture(std::string textureName, unsigned int flags = 0, bool linearFilter = true, bool aniso = true) override;
			TextureBase*		LoadCubemap(std::string xPos, std::string xNeg, std::string yPos, std::string yNeg, std::string zPos, std::string zNeg, unsigned int flags = 0) override;
			MeshMaterial*		LoadMaterial(std::string fileName) override;
			MeshAnimation*		LoadAnimation(std::string fileName) override;

		private:
			std::map<std::string, MeshGeometry*>	loadedMeshes;
			std::map<std::string, MeshMaterial*>	loadedMaterials;
			std::map<std::string, MeshAnimation*>	loadedAnimations;
			std::map<std::string, ShaderBase*>		loadedShaders;
			std::map<std::string, TextureBase*>		loadedTextures;
			std::map<std::string, TextureBase*>		loadedCubeMaps;
		};

	}

}