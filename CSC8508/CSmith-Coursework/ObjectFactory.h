#pragma once

#include "GameTechRenderer.h"

//This class is used to instantiate the various types of gameobject in the game.
//It is used so that there is a single place where meshes/textures/shaders are loaded,
//rather than having to load them in for every single object type.

namespace NCL {
	namespace CSC8508 {

		class BoidObject;
		class BoidSwarm;
		class PlayerObject;
		class AIObject;
		class Game;

		class ObjectFactory {

		public:
			ObjectFactory();
			~ObjectFactory();



			GameObject*		CreateAABBCube(GameWorld* world, Vector3 position, Vector3 dimensions, float inverseMass = 10.0f, bool isStatic = false) const;
			GameObject*		CreateOBBCube(GameWorld* world, Vector3 position, Quaternion orientation, Vector3 dimensions, float inverseMass = 10.0f, bool respawning = false, bool isStatic = false) const;
			GameObject*		CreateSphere(GameWorld* world, Vector3 position, float radius, float inverseMass = 10.0f, bool respawning = false, bool isStatic = false) const;
			GameObject*		CreateCapsule(GameWorld* world, Vector3 position, Quaternion orientation, float halfHeight, float radius, float inverseMass = 10.0f, bool respawning = false, bool isStatic = false) const;

			GameObject*		CreateFloor(GameWorld* world, Vector3 position, Vector2 dimensions) const;
			GameObject*		CreateOrientedFloor(GameWorld* world, Vector3 position, Quaternion orientation, Vector2 dimensions,std::string name = "orientedFloor") const;
			GameObject*		CreateSlipperyFloor(GameWorld* world, const Vector3& position, const Quaternion& orientation, const Vector2& dimensions) const;
			GameObject*		CreateSwampFloor(GameWorld* world, const Vector3& position,const Quaternion& orientation,const Vector2& dimensions) const;

			GameObject*		CreateAnchor(GameWorld* world, const Vector3& position) const;
			GameObject*		CreatePathBlocker(GameWorld* world, const Vector2& position, const Vector2& halfDimensions) const;

			GameObject*		CreateSpinningBlock(GameWorld* world, const Vector3& position, const Vector3& upVector, float force, float length = 10.0f) const;

			GameObject*		CreateBouncePad(GameWorld* world, const Vector3& position, const Quaternion& orientation, float strength = 10.0f, const Vector2& dimensions = { 1,1 }) const;
			GameObject*		CreateTreadmill(GameWorld* world, const Vector3& position, const Quaternion& orientation, float strength = 0.1f, const Vector2& dimensions = { 1,2 }) const;

			GameObject*		CreateScoreBonus(GameWorld* world, Vector3 position, float respawnDelay = 5.0f) const;

			PlayerObject*	CreatePlayer(Game* game, const Vector3& position) const;
			AIObject*		CreateRaceAI(Game* game, const Vector3& position,std::string name,float coinDetectionRange = 10.0f,float maxCoinDistance = 30.0f, float angerThreshold = 3.0f, float strength = 2.0f) const;
			AIObject*		CreateKatamariAI(Game* game, const Vector3& position,std::string name) const;
			BoidObject*		CreateBoid(Game* game, BoidSwarm** swarm, const Vector3& position) const;

		protected:

			OGLMesh* capsuleMesh = nullptr;
			OGLMesh* cubeMesh = nullptr;
			OGLMesh* sphereMesh = nullptr;
			OGLTexture* basicTex = nullptr;
			OGLShader* basicShader = nullptr;

			//Coursework Meshes
			OGLMesh* charMeshA = nullptr;
			OGLMesh* charMeshB = nullptr;
			OGLMesh* enemyMesh = nullptr;
			OGLMesh* coinMesh = nullptr;
			OGLMesh* bonusMesh = nullptr;
		};
	}
}




