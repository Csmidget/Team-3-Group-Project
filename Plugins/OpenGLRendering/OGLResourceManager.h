#pragma once

#include <map>
#include <string>

namespace NCL {

	namespace Rendering {

		class OGLMesh;
		class OGLShader;
		class OGLTexture;

		class OGLResourceManager {

		public:

			OGLMesh*	LoadMesh();
			OGLShader*	LoadShader(std::string shaderVert, std::string shaderFrag, std::string shaderGeom = "");
			OGLTexture*	LoadTexture(std::string textureName, unsigned int flags = 0, bool linearFilter = true, bool aniso = true);
			OGLTexture	LoadCubemap(std::string xPos, std::string xNeg, std::string yPos, std::string yNeg, std::string zPos, std::string zNeg, unsigned int flags = 0);

		private:
			std::map<std::string, OGLMesh*> loadedMeshes;
			std::map<std::string, OGLShader*> loadedShaders;
			std::map<std::string, OGLTexture*> loadedTextures;
			std::map<std::string, OGLTexture*> loadedCubeMaps;
		};

	}

}