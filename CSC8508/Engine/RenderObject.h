#pragma once
#include "../../Common/Matrix4.h"
#include "../../Common/TextureBase.h"
#include "../../Common/ShaderBase.h"
#include "../../Common/Vector4.h"
#include <vector>

namespace NCL {
	using namespace NCL::Rendering;

	class MeshGeometry;
	class MeshMaterial;
	class MeshAnimation;

	namespace CSC8508 {
		class Transform;
		using namespace Maths;

		class RenderObject
		{
		public:
			RenderObject(Transform* parentTransform, MeshGeometry* mesh, MeshMaterial* mat, TextureBase* tex, MeshAnimation* anim, ShaderBase* shader);
			~RenderObject();

			void SetDefaultTexture(TextureBase* t) {
				texture = t;
			}

			TextureBase* GetDefaultTexture() const {
				return texture;
			}

			MeshGeometry*	GetMesh() const {
				return mesh;
			}

			MeshMaterial* GetMaterial() const {
				return material;
			}
			
			MeshAnimation* GetAnimation() const {
				return animation;
			}

			void SetAnimation(MeshAnimation* anim);

			Transform*		GetTransform() const {
				return transform;
			}

			ShaderBase*		GetShader() const {
				return shader;
			}

			void SetColour(const Vector4& c) {
				colour = c;
			}

			Vector4 GetColour() const {
				return colour;
			}

		protected:
			MeshGeometry*	mesh;
			MeshMaterial*	material;
			MeshAnimation*	animation;
			TextureBase*	texture;
			ShaderBase*		shader;
			Transform*		transform;
			Vector4			colour;

			int currentFrame;
			std::vector<Matrix4> animRelativeJoints;
			std::vector<Matrix4> frameMatrices;
		};
	}
}
