#pragma once

namespace NCL {

	namespace Rendering {
		class OGLShader;
	}

	namespace CSC8508 {
		class PointLight;
		class SpotLight;

		class Shadow
		{
		public:
			Shadow();

			unsigned int getCubemapTexture();
			unsigned int get2DmapTexture();
			void BindShadowFBOAsCubemap();
			void BindShadowFBOAs2D();

			void DrawPointLightShadowMap(NCL::Rendering::OGLShader* shadowshader, PointLight& pointlight, int index);
			void DrawSpotLightShadowMap(NCL::Rendering::OGLShader* shadowshader, SpotLight& pointlight, int index);
		protected:
			unsigned depthMapFBO;
			unsigned depthCubemap;
			unsigned depth2Dmap;
		};
	}
}



