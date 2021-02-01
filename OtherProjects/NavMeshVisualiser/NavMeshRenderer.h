#pragma once
#include "../../Plugins/OpenGLRendering/OGLRenderer.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"

namespace NCL {
	class Camera;
	class MeshAnimation;

	class NavMeshRenderer : public OGLRenderer
	{
	public:
		NavMeshRenderer();
		virtual ~NavMeshRenderer();
		void Update(float dt) override;

	protected:
		void RenderFrame()	override;
		OGLMesh*	navMesh;
		OGLShader*	navShader;
		Camera*		camera;
	};
}

