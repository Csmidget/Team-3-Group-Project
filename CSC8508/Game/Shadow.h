#pragma once

#include <../../Assets/packages/glm.0.9.9.800/build/native/include/glm/glm.hpp>
#include <../../Assets/packages/glm.0.9.9.800/build/native/include/glm/gtc/matrix_transform.hpp>
#include <../../Assets/packages/glm.0.9.9.800/build/native/include/glm/gtc/type_ptr.hpp>
#include "GameTechRenderer.h"
#include "PointLight.h"

class Shadow
{
public:
	Shadow();

	unsigned int getAttachmentCubeTexture();
	void DrawPointLightShadowMap(NCL::Rendering::OGLShader* shadowshader, PointLight& pointlight, int width, int height, int index);
protected:
	unsigned depthMapFBO;
	unsigned depthCubemap;
};

