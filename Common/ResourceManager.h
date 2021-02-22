#pragma once

#include <string>
#include <map>

namespace NCL {

	class MeshGeometry;

	namespace Rendering {
	
		class TextureBase;
		class ShaderBase;

		class ResourceManager {

		public:
			virtual MeshGeometry* LoadMesh(std::string fileName) = 0;
			virtual ShaderBase* LoadShader(std::string shaderVert, std::string shaderFrag, std::string shaderGeom = "") = 0;
			virtual TextureBase* LoadTexture(std::string textureName, unsigned int flags = 0, bool linearFilter = true, bool aniso = true) = 0;
			virtual TextureBase* LoadCubemap(std::string xPos, std::string xNeg, std::string yPos, std::string yNeg, std::string zPos, std::string zNeg, unsigned int flags = 0) = 0;
		};
	}
}