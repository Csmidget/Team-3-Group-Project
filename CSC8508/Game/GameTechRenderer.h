#pragma once
#include "../Engine/GameWorld.h"

#include "../../Plugins/OpenGLRendering/OGLRenderer.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"

#include "../../Common/ResourceManager.h"


namespace NCL {
	class Maths::Vector3;
	class Maths::Vector4;
	namespace CSC8508 {
		class RenderObject;

		class Light;

		class GameTechRenderer : public OGLRenderer	{
		public:
			GameTechRenderer(GameWorld& world, ResourceManager& resourceManager);
			~GameTechRenderer();

			OGLShader* getTempShader() { return m_temp_shader; }
			//NCL::Rendering::ResourceManager* GetResourceManager() { return resourceManager; }
		protected:
			void RenderFrame()	override;

			Matrix4 SetupDebugLineMatrix()	const override;
			Matrix4 SetupDebugStringMatrix()const override;

			OGLShader*	defaultShader;
			//GameTechRenderer* renderer;
			GameWorld&	gameWorld;

			void BuildObjectList();
			void SortObjectList();
			void RenderShadowMap();
			void RenderCamera(); 
			void RenderSkybox();
			void InitLight();
			//void LoadLight();
			void LoadSkybox();
			//NCL::Rendering::ResourceManager* resourceManager;
			vector<const RenderObject*> activeObjects;

			OGLShader*  skyboxShader;
			OGLMesh*	skyboxMesh;
			GLuint		skyboxTex;

			OGLShader* m_temp_shader = nullptr;

			//shadow mapping things
			NCL::Rendering::OGLShader*	shadowShader;
			NCL::Rendering::OGLShader* shader;
			GLuint		shadowTex;
			GLuint		shadowFBO;
			Matrix4     shadowMatrix;

			Vector4		lightColour;
			float		lightRadius;
			Vector3		lightPosition;
		};
	}
}

