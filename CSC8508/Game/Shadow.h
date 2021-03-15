#pragma once

#include "../../include/glm/glm.hpp"
#include "../../include/glm/gtc/matrix_transform.hpp"
#include "../../include/glm/gtc/type_ptr.hpp"
#include "GameTechRenderer.h"
#include "PointLight.h"
#include "SpotLight.h"

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

