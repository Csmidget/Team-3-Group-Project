#include "CollisionDetection.h"
#include "CollisionVolume.h"
#include "AABBVolume.h"
#include "OBBVolume.h"
#include "SphereVolume.h"
#include "../../Common/Vector2.h"
#include "../../Common/Window.h"
#include "../../Common/Maths.h"
#include "Debug.h"

#include <list>

using namespace NCL;

Vector3 CollisionDetection::ProjectPointOntoLineSegment(Vector3 lineStart, Vector3 lineEnd, Vector3 point) {

	Vector3 line = lineEnd - lineStart;

	float t = Vector3::Dot(point - lineStart, line) / line.LengthSquared();
	t = Clamp(t, 0.0f, 1.0f);

	return lineStart + (line * t);
}

bool CollisionDetection::RayPlaneIntersection(const Ray&r, const Plane&p, RayCollision& collisions) {
	float ln = Vector3::Dot(p.GetNormal(), r.GetDirection());

	if (ln == 0.0f) {
		return false; //direction vectors are perpendicular!
	}
	
	Vector3 planePoint = p.GetPointOnPlane();

	Vector3 pointDir = planePoint - r.GetPosition();

	float d = Vector3::Dot(pointDir, p.GetNormal()) / ln;

	collisions.collidedAt = r.GetPosition() + (r.GetDirection() * d);

	return true;
}

bool CollisionDetection::RayIntersection(const Ray& r,GameObject& object, RayCollision& collision) {
	bool hasCollided = false;

	const Transform& worldTransform = object.GetTransform();
	const CollisionVolume* volume	= object.GetBoundingVolume();

	if (!volume || !volume->IsActive()) {
		return false;
	}

	switch (volume->type) {
		case VolumeType::AABB:		hasCollided = RayAABBIntersection(r, worldTransform, (const AABBVolume&)*volume	, collision); break;
		case VolumeType::OBB:		hasCollided = RayOBBIntersection(r, worldTransform, (const OBBVolume&)*volume	, collision); break;
		case VolumeType::Sphere:	hasCollided = RaySphereIntersection(r, worldTransform, (const SphereVolume&)*volume	, collision); break;
		case VolumeType::Capsule:	hasCollided = RayCapsuleIntersection(r, worldTransform, (const CapsuleVolume&)*volume, collision); break;
	}

	return hasCollided;
}

bool CollisionDetection::RayBoxIntersection(const Ray&r, const Vector3& boxPos, const Vector3& boxSize, RayCollision& collision, bool includeInternal) {
	Vector3 boxMin = boxPos - boxSize;
	Vector3 boxMax = boxPos + boxSize;

	Vector3 rayPos = r.GetPosition();
	Vector3 rayDir = r.GetDirection();

	if (includeInternal && AABBTest(r.GetPosition(), boxPos, Vector3(0, 0, 0), boxSize)) {
			return true;
	}

	Vector3 tVals(-1, -1, -1);

	for (int i = 0; i < 3; ++i) {
		if (rayDir[i] > 0) {
			tVals[i] = (boxMin[i] - rayPos[i]) / rayDir[i];
		}
		else if (rayDir[i] < 0) {
			tVals[i] = (boxMax[i] - rayPos[i]) / rayDir[i];
		}
	}

	float bestT = tVals.GetMaxElement();
	if (bestT < 0.0f) {
		return false;
	}

	Vector3 intersection = rayPos + (rayDir * bestT);
	const float epsilon = 0.0001f;

	for (int i = 0; i < 3; ++i) {
		if (intersection[i] + epsilon < boxMin[i] ||
			intersection[i] - epsilon > boxMax[i]) {
			return false;
		}
	}
	collision.collidedAt = intersection;
	collision.rayDistance = bestT;

	return true;
}

bool CollisionDetection::RayAABBIntersection(const Ray&r, const Transform& worldTransform, const AABBVolume& volume, RayCollision& collision) {
	Vector3 boxPos = worldTransform.GetPosition();
	Vector3 boxSize = volume.GetHalfDimensions();
	return RayBoxIntersection(r, boxPos, boxSize, collision);
}

bool CollisionDetection::RayOBBIntersection(const Ray&r, const Transform& worldTransform, const OBBVolume& volume, RayCollision& collision) {
	Quaternion orientation = worldTransform.GetOrientation();
	Vector3 position = worldTransform.GetPosition();

	Matrix3 transform = Matrix3(orientation);
	Matrix3 invTransform = Matrix3(orientation.Conjugate());

	Vector3 localRayPos = r.GetPosition() - position;

	Ray tempRay(invTransform * localRayPos, invTransform * r.GetDirection());

	bool collided = RayBoxIntersection(tempRay, Vector3(), volume.GetHalfDimensions(), collision);
	if (collided) {
		collision.collidedAt = transform * collision.collidedAt + position;
	}

	return collided;
}

bool CollisionDetection::RayCapsuleIntersection(const Ray& r, const Transform& worldTransform, const CapsuleVolume& volume, RayCollision& collision) {

	float radius = volume.GetRadius();
	Vector3 capsulePos = worldTransform.GetPosition();
	Vector3 up = worldTransform.GetOrientation() * Vector3(0, 1, 0);

	if ((capsulePos - r.GetPosition()).Length() < radius)
		return false;

	Vector3 upperSpherePos = capsulePos + up * (volume.GetHalfHeight() - radius);
	Vector3 lowerSpherePos = capsulePos - up * (volume.GetHalfHeight() - radius);
	Vector3 crossVec = capsulePos + Vector3::Cross(upperSpherePos - capsulePos, r.GetPosition() - capsulePos);

	Plane p = Plane::PlaneFromTri(capsulePos, upperSpherePos, crossVec);
	float t = (-(Vector3::Dot(r.GetPosition(), p.GetNormal()) + p.GetDistance())) / (Vector3::Dot(r.GetDirection(), p.GetNormal()));

	//Point where ray intersects the plane
	Vector3 pIntersect = r.GetPosition() + (r.GetDirection() * t);
	float dist;
	
	if (Vector3::Dot(-up, pIntersect - upperSpherePos) < 0) {
		dist = (pIntersect - upperSpherePos).Length();
	}
	else if (Vector3::Dot(up, pIntersect - lowerSpherePos) < 0) {
		dist = (pIntersect - lowerSpherePos).Length();
	}
	else {
		Vector3 lineProjection = capsulePos + up * (Vector3::Dot(pIntersect - capsulePos, up));
		dist = (pIntersect - lineProjection).Length();
	}

	if (radius > dist) {
		float intersectDist = (r.GetPosition() - pIntersect).Length();
		float offset = sqrt((radius * radius) - (dist * dist));
		collision.rayDistance = intersectDist - (offset);
		collision.collidedAt = r.GetPosition() + (r.GetDirection() * collision.rayDistance);
		return true;
	}

	if ((upperSpherePos - r.GetPosition()).LengthSquared() < (lowerSpherePos - r.GetPosition()).LengthSquared()) {
		return RaySphereIntersection(r, upperSpherePos, radius, collision) || RaySphereIntersection(r, lowerSpherePos, radius, collision);
	}
	else {
		return RaySphereIntersection(r, lowerSpherePos, radius, collision) || RaySphereIntersection(r, upperSpherePos, radius, collision);
	}
}


bool CollisionDetection::RaySphereIntersection(const Ray& r, const Transform& worldTransform, const SphereVolume& volume, RayCollision& collision) {
	Vector3 spherePos = worldTransform.GetPosition();
	float sphereRadius = volume.GetRadius();

	return RaySphereIntersection(r, spherePos, sphereRadius, collision);
}

bool CollisionDetection::RaySphereIntersection(const Ray& r, const Vector3& spherePosition, const float sphereRadius, RayCollision& collision) {

	//Get the direction between the ray origin and the sphere origin
	Vector3 dir = (spherePosition - r.GetPosition());

	//ADDITION Our ray coming from inside the sphere, so do not collide.
	if (dir.Length() < sphereRadius) {
		return false;
	}

	//Project the sphere's origin onto our ray direction vector
	float sphereProj = Vector3::Dot(dir, r.GetDirection());

	if (sphereProj < 0.0f) {
		return false;
	}

	//Get closest point on ray line to sphere
	Vector3 point = r.GetPosition() + (r.GetDirection() * sphereProj);

	float sphereDist = (point - spherePosition).Length();

	if (sphereDist > sphereRadius) {
		return false;
	}

	float offset = sqrt((sphereRadius * sphereRadius) - (sphereDist * sphereDist));

	collision.rayDistance = sphereProj - (offset);
	collision.collidedAt = r.GetPosition() + (r.GetDirection() * collision.rayDistance);

	return true;
}

Matrix4 GenerateInverseView(const Camera &c) {
	float pitch = c.GetPitch();
	float yaw	= c.GetYaw();
	Vector3 position = c.GetPosition();

	Matrix4 iview =
		Matrix4::Translation(position) *
		Matrix4::Rotation(-yaw, Vector3(0, -1, 0)) *
		Matrix4::Rotation(-pitch, Vector3(-1, 0, 0));

	return iview;
}

Vector3 CollisionDetection::Unproject(const Vector3& screenPos, const Camera& cam) {
	Vector2 screenSize = Window::GetWindow()->GetScreenSize();

	float aspect	= screenSize.x / screenSize.y;
	float fov		= cam.GetFieldOfVision();
	float nearPlane = cam.GetNearPlane();
	float farPlane  = cam.GetFarPlane();

	//Create our inverted matrix! Note how that to get a correct inverse matrix,
	//the order of matrices used to form it are inverted, too.
	Matrix4 invVP = GenerateInverseView(cam) * GenerateInverseProjection(aspect, fov, nearPlane, farPlane);

	//Our mouse position x and y values are in 0 to screen dimensions range,
	//so we need to turn them into the -1 to 1 axis range of clip space.
	//We can do that by dividing the mouse values by the width and height of the
	//screen (giving us a range of 0.0 to 1.0), multiplying by 2 (0.0 to 2.0)
	//and then subtracting 1 (-1.0 to 1.0).
	Vector4 clipSpace = Vector4(
		(screenPos.x / (float)screenSize.x) * 2.0f - 1.0f,
		(screenPos.y / (float)screenSize.y) * 2.0f - 1.0f,
		(screenPos.z),
		1.0f
	);

	//Then, we multiply our clipspace coordinate by our inverted matrix
	Vector4 transformed = invVP * clipSpace;

	//our transformed w coordinate is now the 'inverse' perspective divide, so
	//we can reconstruct the final world space by dividing x,y,and z by w.
	return Vector3(transformed.x / transformed.w, transformed.y / transformed.w, transformed.z / transformed.w);
}

Ray CollisionDetection::BuildRayFromMouse(const Camera& cam) {
	Vector2 screenMouse = Window::GetMouse()->GetAbsolutePosition();
	Vector2 screenSize	= Window::GetWindow()->GetScreenSize();

	//We remove the y axis mouse position from height as OpenGL is 'upside down',
	//and thinks the bottom left is the origin, instead of the top left!
	Vector3 nearPos = Vector3(screenMouse.x,
		screenSize.y - screenMouse.y,
		-0.99999f
	);

	//We also don't use exactly 1.0 (the normalised 'end' of the far plane) as this
	//causes the unproject function to go a bit weird. 
	Vector3 farPos = Vector3(screenMouse.x,
		screenSize.y - screenMouse.y,
		0.99999f
	);

	Vector3 a = Unproject(nearPos, cam);
	Vector3 b = Unproject(farPos, cam);
	Vector3 c = b - a;

	c.Normalise();

	//std::cout << "Ray Direction:" << c << std::endl;

	return Ray(cam.GetPosition(), c);
}

//http://bookofhook.com/mousepick.pdf
Matrix4 CollisionDetection::GenerateInverseProjection(float aspect, float fov, float nearPlane, float farPlane) {
	Matrix4 m;

	float t = tan(fov*PI_OVER_360);

	float neg_depth = nearPlane - farPlane;

	const float h = 1.0f / t;

	float c = (farPlane + nearPlane) / neg_depth;
	float e = -1.0f;
	float d = 2.0f*(nearPlane*farPlane) / neg_depth;

	m.array[0]  = aspect / h;
	m.array[5]  = tan(fov*PI_OVER_360);

	m.array[10] = 0.0f;
	m.array[11] = 1.0f / d;

	m.array[14] = 1.0f / e;

	m.array[15] = -c / (d*e);

	return m;
}

/*
And here's how we generate an inverse view matrix. It's pretty much
an exact inversion of the BuildViewMatrix function of the Camera class!
*/
Matrix4 CollisionDetection::GenerateInverseView(const Camera &c) {
	float pitch = c.GetPitch();
	float yaw	= c.GetYaw();
	Vector3 position = c.GetPosition();

	Matrix4 iview =
Matrix4::Translation(position) *
Matrix4::Rotation(yaw, Vector3(0, 1, 0)) *
Matrix4::Rotation(pitch, Vector3(1, 0, 0));

return iview;
}


/*
If you've read through the Deferred Rendering tutorial you should have a pretty
good idea what this function does. It takes a 2D position, such as the mouse
position, and 'unprojects' it, to generate a 3D world space position for it.

Just as we turn a world space position into a clip space position by multiplying
it by the model, view, and projection matrices, we can turn a clip space
position back to a 3D position by multiply it by the INVERSE of the
view projection matrix (the model matrix has already been assumed to have
'transformed' the 2D point). As has been mentioned a few times, inverting a
matrix is not a nice operation, either to understand or code. But! We can cheat
the inversion process again, just like we do when we create a view matrix using
the camera.

So, to form the inverted matrix, we need the aspect and fov used to create the
projection matrix of our scene, and the camera used to form the view matrix.

*/
Vector3	CollisionDetection::UnprojectScreenPosition(Vector3 position, float aspect, float fov, const Camera &c) {
	//Create our inverted matrix! Note how that to get a correct inverse matrix,
	//the order of matrices used to form it are inverted, too.
	Matrix4 invVP = GenerateInverseView(c) * GenerateInverseProjection(aspect, fov, c.GetNearPlane(), c.GetFarPlane());

	Vector2 screenSize = Window::GetWindow()->GetScreenSize();

	//Our mouse position x and y values are in 0 to screen dimensions range,
	//so we need to turn them into the -1 to 1 axis range of clip space.
	//We can do that by dividing the mouse values by the width and height of the
	//screen (giving us a range of 0.0 to 1.0), multiplying by 2 (0.0 to 2.0)
	//and then subtracting 1 (-1.0 to 1.0).
	Vector4 clipSpace = Vector4(
		(position.x / (float)screenSize.x) * 2.0f - 1.0f,
		(position.y / (float)screenSize.y) * 2.0f - 1.0f,
		(position.z) - 1.0f,
		1.0f
	);

	//Then, we multiply our clipspace coordinate by our inverted matrix
	Vector4 transformed = invVP * clipSpace;

	//our transformed w coordinate is now the 'inverse' perspective divide, so
	//we can reconstruct the final world space by dividing x,y,and z by w.
	return Vector3(transformed.x / transformed.w, transformed.y / transformed.w, transformed.z / transformed.w);
}

bool CollisionDetection::ObjectIntersection(GameObject* a, GameObject* b, CollisionInfo& collisionInfo) {
	const CollisionVolume* volA = a->GetBoundingVolume();
	const CollisionVolume* volB = b->GetBoundingVolume();

	if (!volA || !volB || !volA->IsActive() || !volB->IsActive()) {
		return false;
	}

	collisionInfo.a = a;
	collisionInfo.b = b;

	Transform& transformA = a->GetTransform();
	Transform& transformB = b->GetTransform();

	VolumeType pairType = (VolumeType)((int)volA->type | (int)volB->type);

	if (pairType == VolumeType::AABB) {
		return AABBIntersection((AABBVolume&)*volA, transformA, (AABBVolume&)*volB, transformB, collisionInfo);
	}

	if (pairType == VolumeType::Sphere) {
		return SphereIntersection((SphereVolume&)*volA, transformA, (SphereVolume&)*volB, transformB, collisionInfo);
	}

	if (pairType == VolumeType::OBB) {
		return OBBIntersection((OBBVolume&)*volA, transformA, (OBBVolume&)*volB, transformB, collisionInfo);
	}

	if (pairType == VolumeType::Capsule) {
		return CapsuleIntersection((CapsuleVolume&)*volA, transformA, (CapsuleVolume&)*volB, transformB, collisionInfo);
	}

	if (volA->type == VolumeType::AABB && volB->type == VolumeType::Sphere) {
		return AABBSphereIntersection((AABBVolume&)*volA, transformA, (SphereVolume&)*volB, transformB, collisionInfo);
	}
	if (volA->type == VolumeType::Sphere && volB->type == VolumeType::AABB) {
		collisionInfo.a = b;
		collisionInfo.b = a;
		return AABBSphereIntersection((AABBVolume&)*volB, transformB, (SphereVolume&)*volA, transformA, collisionInfo);
	}

	if (volA->type == VolumeType::Capsule && volB->type == VolumeType::Sphere) {
		return SphereCapsuleIntersection((CapsuleVolume&)*volA, transformA, (SphereVolume&)*volB, transformB, collisionInfo);
	}
	if (volA->type == VolumeType::Sphere && volB->type == VolumeType::Capsule) {
		collisionInfo.a = b;
		collisionInfo.b = a;
		return SphereCapsuleIntersection((CapsuleVolume&)*volB, transformB, (SphereVolume&)*volA, transformA, collisionInfo);
	}

	if (volA->type == VolumeType::AABB && volB->type == VolumeType::Capsule) {
		return AABBCapsuleIntersection((AABBVolume&)*volA, transformA, (CapsuleVolume&)*volB, transformB, collisionInfo);
	}
	if (volA->type == VolumeType::Capsule && volB->type == VolumeType::AABB) {
		collisionInfo.a = b;
		collisionInfo.b = a;
		return AABBCapsuleIntersection((AABBVolume&)*volB, transformB, (CapsuleVolume&)*volA, transformA, collisionInfo);
	}

	if (volA->type == VolumeType::AABB && volB->type == VolumeType::OBB) {
		return AABBOBBIntersection((AABBVolume&)*volA, transformA, (OBBVolume&)*volB, transformB, collisionInfo);
	}
	if (volA->type == VolumeType::OBB && volB->type == VolumeType::AABB) {
		collisionInfo.a = b;
		collisionInfo.b = a;
		return AABBOBBIntersection((AABBVolume&)*volB, transformB, (OBBVolume&)*volA, transformA, collisionInfo);
	}

	if (volA->type == VolumeType::OBB && volB->type == VolumeType::Sphere) {
		return OBBSphereIntersection((OBBVolume&)*volA, transformA, (SphereVolume&)*volB, transformB, collisionInfo);
	}
	if (volA->type == VolumeType::Sphere && volB->type == VolumeType::OBB) {
		collisionInfo.a = b;
		collisionInfo.b = a;
		return OBBSphereIntersection((OBBVolume&)*volB, transformB, (SphereVolume&)*volA, transformA, collisionInfo);
	}

	if (volA->type == VolumeType::OBB && volB->type == VolumeType::Capsule) {
		return OBBCapsuleIntersection((OBBVolume&)*volA, transformA, (CapsuleVolume&)*volB, transformB, collisionInfo);
	}
	if (volA->type == VolumeType::Capsule && volB->type == VolumeType::OBB) {
		collisionInfo.a = b;
		collisionInfo.b = a;
		return OBBCapsuleIntersection((OBBVolume&)*volB, transformB, (CapsuleVolume&)*volA, transformA, collisionInfo);
	}

	return false;
}

bool CollisionDetection::AABBTest(const Vector3& posA, const Vector3& posB, const Vector3& halfSizeA, const Vector3& halfSizeB) {
	
	Vector3 delta = posB - posA;
	Vector3 totalSize = halfSizeA + halfSizeB;

	if (abs(delta.x) < totalSize.x &&
		abs(delta.y) < totalSize.y &&
		abs(delta.z) < totalSize.z) {
		return true;
	}
	
	return false;
}

//AABB/AABB Collisions
bool CollisionDetection::AABBIntersection(const AABBVolume& volumeA, const Transform& worldTransformA,
	const AABBVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {

	Vector3 boxAPos = worldTransformA.GetPosition();
	Vector3 boxBPos = worldTransformB.GetPosition();

	Vector3 boxASize = volumeA.GetHalfDimensions();
	Vector3 boxBSize = volumeB.GetHalfDimensions();

	bool overlap = AABBTest(boxAPos, boxBPos, boxASize, boxBSize);

	if (overlap) {
		static const Vector3 faces[6] = {
			Vector3(-1,0,0),Vector3(1,0,0),
			Vector3(0,-1,0),Vector3(0,1,0),
			Vector3(0,0,-1),Vector3(0,0,1)
		};

		Vector3 maxA = boxAPos + boxASize;
		Vector3 minA = boxAPos - boxASize;

		Vector3 maxB = boxBPos + boxBSize;
		Vector3 minB = boxBPos - boxBSize;

		float distance[6] = {
			(maxB.x - minA.x), //left
			(maxA.x - minB.x), //right
			(maxB.y - minA.y), //bottom
			(maxA.y - minB.y), //top
			(maxB.z - minA.z), //far 
			(maxA.z - minB.z), //near
		};

		float penetration = FLT_MAX;

		Vector3 bestAxis;

		for (int i = 0; i < 6; i++) {
			if (distance[i] < penetration) {
				penetration = distance[i];
				bestAxis = faces[i];
			}
		}

		collisionInfo.AddContactPoint(Vector3(), Vector3(), bestAxis, penetration);
		return true;
	}

	return false;
}

//Sphere / Sphere Collision
bool CollisionDetection::SphereIntersection(const float radiusA, const Vector3& positionA, const float radiusB, const Vector3& positionB, CollisionInfo& collisionInfo) {
	float radii = radiusA + radiusB;
	Vector3 delta = positionB - positionA;

	float deltaLength = delta.Length();

	if (deltaLength < radii) {
		float penetration = (radii - deltaLength);
		Vector3 normal = delta.Normalised();
		Vector3 localA = normal * radiusA;
		Vector3 localB = -normal * radiusB;

		collisionInfo.AddContactPoint(localA, localB, normal, penetration);
		return true;
	}

	return false;
}

bool CollisionDetection::SphereIntersection(const SphereVolume& volumeA, const Transform& worldTransformA,
	const SphereVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {
	return SphereIntersection(volumeA.GetRadius(), worldTransformA.GetPosition(), volumeB.GetRadius(), worldTransformB.GetPosition(),collisionInfo);
}

//Capsule / Capsule Collision
bool CollisionDetection::CapsuleIntersection(const CapsuleVolume& volumeA, const Transform& worldTransformA, const CapsuleVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {
	//Based on: https://wickedengine.net/2020/04/26/capsule-collision-detection/

	Vector3 aPos = worldTransformA.GetPosition();
	Vector3 bPos = worldTransformB.GetPosition();

	float aHeight = volumeA.GetHalfHeight();
	float bHeight = volumeB.GetHalfHeight();

	float aRadius = volumeA.GetRadius();
	float bRadius = volumeB.GetRadius();

	Vector3 aUp		= worldTransformA.GetOrientation() * Vector3(0, 1, 0);
	Vector3 aTop	= aPos + aUp * (aHeight - aRadius);
	Vector3 aBottom = aPos - aUp * (aHeight - aRadius);

	Vector3 bUp		= worldTransformB.GetOrientation() * Vector3(0, 1, 0);
	Vector3 bTop	= bPos + bUp * (bHeight - bRadius);
	Vector3 bBottom = bPos - bUp * (bHeight - bRadius);

	Vector3 v0 = bBottom - aBottom;
	Vector3 v1 = bTop - aBottom;
	Vector3 v2 = bBottom - aTop;
	Vector3 v3 = bTop - aTop;

	float d0 = v0.LengthSquared();
	float d1 = v1.LengthSquared();
	float d2 = v2.LengthSquared();
	float d3 = v3.LengthSquared();

	Vector3 closestA;
	if ((d2 < d0 && d2 < d1) || (d3 < d0 && d3 < d1)) {
		closestA = aTop;
	}
	else {
		closestA = aBottom;
	}
	
	Vector3 closestB = ProjectPointOntoLineSegment(bBottom, bTop, closestA);
	closestA		 = ProjectPointOntoLineSegment(aBottom, aTop, closestB);

	//Run sphere intersection to check for collision
	if (SphereIntersection(volumeA.GetRadius(), closestA, volumeB.GetRadius(), closestB, collisionInfo)) {
		collisionInfo.point.localA = collisionInfo.point.localA + (closestA - aPos);
		collisionInfo.point.localB = collisionInfo.point.localB + (closestB - bPos);
		return true;
	}

	return false;
}

bool CollisionDetection::BoxSphereIntersection(const Vector3& boxSize, const Vector3& boxPosition,
	const float sphereRadius, const Vector3& spherePosition, CollisionInfo& collisionInfo, bool axisAligned) {

	Vector3 delta = spherePosition - boxPosition;

	Vector3 closestPointOnBox = Maths::Clamp(delta, -boxSize, boxSize);

	Vector3 localPoint = delta - closestPointOnBox;
	float distance = localPoint.Length();
	if (distance < sphereRadius) {
		Vector3 collisionNormal = localPoint.Normalised();
		float penetration = (sphereRadius - distance);

		Vector3 localA = axisAligned ? Vector3() : closestPointOnBox;
		Vector3 localB = -collisionNormal * sphereRadius;

		collisionInfo.AddContactPoint(localA, localB, collisionNormal, penetration);
		return true;
	}

	return false;
}

//AABB - Sphere Collision
bool CollisionDetection::AABBSphereIntersection(const AABBVolume& volumeA, const Transform& worldTransformA,
	const SphereVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {

	return BoxSphereIntersection(volumeA.GetHalfDimensions(), worldTransformA.GetPosition(), volumeB.GetRadius(), worldTransformB.GetPosition(), collisionInfo);
}

//OBB - Sphere Collision
bool CollisionDetection::OBBSphereIntersection(const OBBVolume& volumeA, const Transform& worldTransformA,
	const SphereVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {

	Quaternion boxOrientation = worldTransformA.GetOrientation();
	Vector3 boxPosition = worldTransformA.GetPosition();

	Matrix3 transform = Matrix3(boxOrientation);
	Matrix3 invTransform = Matrix3(boxOrientation.Conjugate());

	Vector3 localSpherePos = invTransform * (worldTransformB.GetPosition() - boxPosition);

	if (BoxSphereIntersection(volumeA.GetHalfDimensions(), Vector3(), volumeB.GetRadius(), localSpherePos, collisionInfo, false)) {
		collisionInfo.point.localA = transform * collisionInfo.point.localA;
		collisionInfo.point.localB = transform * collisionInfo.point.localB;
		collisionInfo.point.normal = transform * collisionInfo.point.normal;
		return true;
	}

	return false;
}

Vector3 OBBSupport(const Transform& worldTransform, Vector3 worldDir) {
	Vector3 localDir = worldTransform.GetOrientation().Conjugate() * worldDir;
	Vector3 vertex;

	//0.5 instead of 1 because cubes are double scale in this program due to a small mesh.
	vertex.x = localDir.x < 0 ? -0.5f : 0.5f;
	vertex.y = localDir.y < 0 ? -0.5f : 0.5f;
	vertex.z = localDir.z < 0 ? -0.5f : 0.5f;

	return worldTransform.GetMatrix() * vertex;
}

bool CollisionDetection::TestBoxesAgainstAxis(const Transform& aTransform, const Transform& bTransform, const Vector3& axisDirection, std::vector<ContactPoint>& contactPoints) {

	//Ignore this collision if the normals are exactly aligned.
	if (axisDirection == Vector3(0, 0, 0))
		return true;

	Vector3 aMinVec = OBBSupport(aTransform, -axisDirection);
	Vector3 aMaxVec = OBBSupport(aTransform, axisDirection);
	Vector3 bMinVec = OBBSupport(bTransform, -axisDirection);
	Vector3 bMaxVec = OBBSupport(bTransform, axisDirection);

	float aMin = Vector3::Dot(axisDirection, aMinVec);
	float aMax = Vector3::Dot(axisDirection, aMaxVec);
	float bMin = Vector3::Dot(axisDirection, bMinVec);
	float bMax = Vector3::Dot(axisDirection, bMaxVec);

	//This definitely doesn't generate the correct contact point in all scenarios, but it's good enough for our use case.
	if (aMin > bMin && aMin <= bMax) {
		ContactPoint contact;
		contact.penetration = bMax - aMin;

		contact.localA = aMinVec - aTransform.GetPosition();
		contact.localB = aMinVec - bTransform.GetPosition() + (axisDirection * contact.penetration);
		contact.normal = axisDirection;
		contactPoints.push_back(contact);
		return true;
	}
	if (bMin > aMin && bMin <= aMax) {
		ContactPoint contact;
		contact.penetration = aMax - bMin;
		contact.localA = bMinVec - aTransform.GetPosition() + (axisDirection * contact.penetration);
		contact.localB = bMinVec - bTransform.GetPosition();
		contact.normal = axisDirection;
		contactPoints.push_back(contact);
		return true;
	}

	return false;
}


bool CollisionDetection::OBBIntersection(
	const OBBVolume& volumeA, const Transform& worldTransformA,
	const OBBVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {

	Vector3 aPos = worldTransformA.GetPosition();
	Quaternion aOrientation = worldTransformA.GetOrientation();
	Vector3 aHalfSize = volumeA.GetHalfDimensions();

	Vector3 bPos = worldTransformB.GetPosition();
	Quaternion bOrientation = worldTransformB.GetOrientation();
	Vector3 bHalfSize = volumeB.GetHalfDimensions();

	const Vector3 aNormals[3] {	
		aOrientation * Vector3(1,0,0),
		aOrientation * Vector3(0,1,0),
		aOrientation * Vector3(0,0,1) 
	};

	const Vector3 bNormals[3]{
		bOrientation * Vector3(1,0,0),
		bOrientation * Vector3(0,1,0),
		bOrientation * Vector3(0,0,1)
	};

	std::vector<ContactPoint> contactPoints;

	if (TestBoxesAgainstAxis(worldTransformA, worldTransformB, aNormals[0], contactPoints) &&
		TestBoxesAgainstAxis(worldTransformA, worldTransformB, aNormals[1], contactPoints) &&
		TestBoxesAgainstAxis(worldTransformA, worldTransformB, aNormals[2], contactPoints) &&
		TestBoxesAgainstAxis(worldTransformA, worldTransformB, bNormals[0], contactPoints) &&
		TestBoxesAgainstAxis(worldTransformA, worldTransformB, bNormals[1], contactPoints) &&
		TestBoxesAgainstAxis(worldTransformA, worldTransformB, bNormals[2], contactPoints) &&
		TestBoxesAgainstAxis(worldTransformA, worldTransformB, Vector3::Cross(aNormals[0], bNormals[0]).Normalised(), contactPoints) &&
		TestBoxesAgainstAxis(worldTransformA, worldTransformB, Vector3::Cross(aNormals[0], bNormals[1]).Normalised(), contactPoints) &&
		TestBoxesAgainstAxis(worldTransformA, worldTransformB, Vector3::Cross(aNormals[0], bNormals[2]).Normalised(), contactPoints) &&
		TestBoxesAgainstAxis(worldTransformA, worldTransformB, Vector3::Cross(aNormals[1], bNormals[0]).Normalised(), contactPoints) &&
		TestBoxesAgainstAxis(worldTransformA, worldTransformB, Vector3::Cross(aNormals[1], bNormals[1]).Normalised(), contactPoints) &&
		TestBoxesAgainstAxis(worldTransformA, worldTransformB, Vector3::Cross(aNormals[1], bNormals[2]).Normalised(), contactPoints) &&
		TestBoxesAgainstAxis(worldTransformA, worldTransformB, Vector3::Cross(aNormals[2], bNormals[0]).Normalised(), contactPoints) &&
		TestBoxesAgainstAxis(worldTransformA, worldTransformB, Vector3::Cross(aNormals[2], bNormals[1]).Normalised(), contactPoints) &&
		TestBoxesAgainstAxis(worldTransformA, worldTransformB, Vector3::Cross(aNormals[2], bNormals[2]).Normalised(), contactPoints)) {

		ContactPoint minPen;
		minPen.penetration = FLT_MAX;
		for (auto cp : contactPoints) {
			if (cp.penetration < minPen.penetration)
				minPen = cp;
		}

		if (Vector3::Dot(minPen.normal, bPos - aPos) < 0) {
			minPen.normal = -minPen.normal;
		}

		//Debug::DrawLine(aPos, bPos,Vector4(1, 1, 1, 1), 1.0f);
		//Debug::DrawLine(aPos + (bPos - aPos) / 2, (aPos + (bPos - aPos) / 2) +  (minPen.normal * 10),Vector4(1,0,0,1),1.0f);
		collisionInfo.point = minPen;

		return true;
	}
		
	return false;
}

bool CollisionDetection::AABBOBBIntersection(
	const AABBVolume& volumeA, const Transform& worldTransformA,
	const OBBVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {

	Vector3 aPos = worldTransformA.GetPosition();
	Vector3 aHalfSize = volumeA.GetHalfDimensions();

	Vector3 bPos = worldTransformB.GetPosition();
	Quaternion bOrientation = worldTransformB.GetOrientation();
	Vector3 bHalfSize = volumeB.GetHalfDimensions();

	const Vector3 aNormals[3]{
		Vector3(1,0,0),
		Vector3(0,1,0),
		Vector3(0,0,1)
	};

	const Vector3 bNormals[3]{
		bOrientation * Vector3(1,0,0),
		bOrientation * Vector3(0,1,0),
		bOrientation * Vector3(0,0,1)
	};

	std::vector<ContactPoint> contactPoints;

	bool intersecting = true;

	intersecting = TestBoxesAgainstAxis(worldTransformA, worldTransformB, aNormals[0], contactPoints) && intersecting;
	intersecting = TestBoxesAgainstAxis(worldTransformA, worldTransformB, aNormals[1], contactPoints) && intersecting;
	intersecting = TestBoxesAgainstAxis(worldTransformA, worldTransformB, aNormals[2], contactPoints) && intersecting;
	intersecting = TestBoxesAgainstAxis(worldTransformA, worldTransformB, bNormals[0], contactPoints) && intersecting;
	intersecting = TestBoxesAgainstAxis(worldTransformA, worldTransformB, bNormals[1], contactPoints) && intersecting;
	intersecting = TestBoxesAgainstAxis(worldTransformA, worldTransformB, bNormals[2], contactPoints) && intersecting;
	intersecting = TestBoxesAgainstAxis(worldTransformA, worldTransformB, Vector3::Cross(aNormals[0], bNormals[0]).Normalised(), contactPoints) && intersecting;
	intersecting = TestBoxesAgainstAxis(worldTransformA, worldTransformB, Vector3::Cross(aNormals[0], bNormals[1]).Normalised(), contactPoints) && intersecting;
	intersecting = TestBoxesAgainstAxis(worldTransformA, worldTransformB, Vector3::Cross(aNormals[0], bNormals[2]).Normalised(), contactPoints) && intersecting;
	intersecting = TestBoxesAgainstAxis(worldTransformA, worldTransformB, Vector3::Cross(aNormals[1], bNormals[0]).Normalised(), contactPoints) && intersecting;
	intersecting = TestBoxesAgainstAxis(worldTransformA, worldTransformB, Vector3::Cross(aNormals[1], bNormals[1]).Normalised(), contactPoints) && intersecting;
	intersecting = TestBoxesAgainstAxis(worldTransformA, worldTransformB, Vector3::Cross(aNormals[1], bNormals[2]).Normalised(), contactPoints) && intersecting;
	intersecting = TestBoxesAgainstAxis(worldTransformA, worldTransformB, Vector3::Cross(aNormals[2], bNormals[0]).Normalised(), contactPoints) && intersecting;
	intersecting = TestBoxesAgainstAxis(worldTransformA, worldTransformB, Vector3::Cross(aNormals[2], bNormals[1]).Normalised(), contactPoints) && intersecting;
	intersecting = TestBoxesAgainstAxis(worldTransformA, worldTransformB, Vector3::Cross(aNormals[2], bNormals[2]).Normalised(), contactPoints) && intersecting;

	if (intersecting) {
		ContactPoint minPen;
		minPen.penetration = FLT_MAX;
		for (auto cp : contactPoints) {
			if (cp.penetration < minPen.penetration)
				minPen = cp;
		}

		minPen.localA = Vector3();

		if (Vector3::Dot(minPen.normal, bPos - aPos) < 0) {
			minPen.normal = -minPen.normal;
		}

		//Debug::DrawLine(aPos, bPos,Vector4(1, 1, 1, 1), 1.0f);
		//Debug::DrawLine(aPos + (bPos - aPos) / 2, (aPos + (bPos - aPos) / 2) +  (minPen.normal * 10),Vector4(1,0,0,1),1.0f);
		collisionInfo.point = minPen;

		return true;
	}

	return false;
}

bool CollisionDetection::SphereCapsuleIntersection(
	const CapsuleVolume& volumeA, const Transform& worldTransformA,
	const SphereVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {

	Vector3 aPos = worldTransformA.GetPosition();
	Vector3 bPos = worldTransformB.GetPosition();

	float aHeight = volumeA.GetHalfHeight();

	float aRadius = volumeA.GetRadius();

	Vector3 aUp = worldTransformA.GetOrientation() * Vector3(0, 1, 0);
	Vector3 aTop = aPos + aUp * (aHeight - aRadius);
	Vector3 aBottom = aPos - aUp * (aHeight - aRadius);

	Vector3 closestPoint = ProjectPointOntoLineSegment(aBottom, aTop, bPos);


	//Run sphere intersection to check for collision
	if (SphereIntersection(volumeA.GetRadius(), closestPoint, volumeB.GetRadius(), bPos, collisionInfo)) {
		collisionInfo.point.localA = collisionInfo.point.localA + (closestPoint - aPos);
		return true;
	}

	return false;
}

//AABB / Capsule Collision, This doesn't work in all cases (capsules much larger than cubes) but is close enough for most use cases.
bool CollisionDetection::AABBCapsuleIntersection(
	const AABBVolume& volumeA, const Transform& worldTransformA,
	const CapsuleVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {

	Vector3 boxPos = worldTransformA.GetPosition();
	Vector3 boxSize = volumeA.GetHalfDimensions();

	Vector3 boxMin = boxPos - boxSize;
	Vector3 boxMax = boxPos + boxSize;

	Vector3 capsulePos = worldTransformB.GetPosition();
	float capsuleRadius = volumeB.GetRadius();
	float capsuleHalfHeight = volumeB.GetHalfHeight();
	Vector3 capsuleUp = worldTransformB.GetOrientation() * Vector3(0, 1, 0);
	Vector3 capsuleTop = capsulePos + capsuleUp * (capsuleHalfHeight - capsuleRadius);
	Vector3 capsuleBottom = capsulePos - capsuleUp * (capsuleHalfHeight - capsuleRadius);

//	Vector3 closestPointOnCapsule = ProjectPointOntoLineSegment(capsuleBottom, capsuleTop, boxPos);
//
//	if (BoxSphereIntersection(boxSize, boxPos, capsuleRadius, closestPointOnCapsule, collisionInfo)) {
//		collisionInfo.point.localB = collisionInfo.point.localB - (closestPointOnCapsule - capsulePos);
//		return true;
//	}

	Vector3 tVals(FLT_MAX, FLT_MAX, FLT_MAX);

	Vector3 posDiff = boxPos - capsulePos;
	for (int i = 0; i < 3; ++i) {
		if (posDiff[i] > 0) {
			tVals[i] = (boxMin[i] - capsulePos[i]) / capsuleUp[i];
		}
		else if (posDiff[i] < 0) {
			tVals[i] = (boxMax[i] - capsulePos[i]) / capsuleUp[i];
		}
	}

	float bestT = tVals.x;
	if (abs(tVals.y) < abs(bestT))
		bestT = tVals.y;
	if (abs(tVals.z) < abs(bestT))
		bestT = tVals.z;

	Vector3 intersection = capsulePos + (capsuleUp * bestT);

	Vector3 closestPointOnBox = Clamp(intersection, boxMin, boxMax);

	Vector3 closestPointOnCapsule = ProjectPointOntoLineSegment(capsuleBottom, capsuleTop, closestPointOnBox);


	if (BoxSphereIntersection(boxSize, boxPos, capsuleRadius, closestPointOnCapsule, collisionInfo)) {
		collisionInfo.point.localB = collisionInfo.point.localB + (closestPointOnCapsule - capsulePos);
		return true;
	}

	return false;
}

//OBB / Capsule Collision, Basically the same as AABB capsule collision.
bool CollisionDetection::OBBCapsuleIntersection(
	const OBBVolume& volumeA, const Transform& worldTransformA,
	const CapsuleVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo) {

	Vector3 boxPos = worldTransformA.GetPosition();
	Vector3 boxSize = volumeA.GetHalfDimensions();
	Quaternion boxOrientation = worldTransformA.GetOrientation();

	Matrix3 transform = Matrix3(boxOrientation);
	Matrix3 invTransform = Matrix3(boxOrientation.Conjugate());

	Vector3 boxMin = -boxSize;
	Vector3 boxMax = boxSize;

	Vector3 capsulePos = invTransform * (worldTransformB.GetPosition() - boxPos);
	float capsuleRadius = volumeB.GetRadius();
	float capsuleHalfHeight = volumeB.GetHalfHeight();
	Vector3 capsuleUp = invTransform * (worldTransformB.GetOrientation() * Vector3(0, 1, 0));
	Vector3 capsuleTop = capsulePos + capsuleUp * (capsuleHalfHeight - capsuleRadius);
	Vector3 capsuleBottom = capsulePos - capsuleUp * (capsuleHalfHeight - capsuleRadius);

	//	Vector3 closestPointOnCapsule = ProjectPointOntoLineSegment(capsuleBottom, capsuleTop, boxPos);
	//
	//	if (BoxSphereIntersection(boxSize, boxPos, capsuleRadius, closestPointOnCapsule, collisionInfo)) {
	//		collisionInfo.point.localB = collisionInfo.point.localB - (closestPointOnCapsule - capsulePos);
	//		return true;
	//	}

	Vector3 tVals(FLT_MAX, FLT_MAX, FLT_MAX);

	Vector3 posDiff = -capsulePos;
	for (int i = 0; i < 3; ++i) {
		if (posDiff[i] > 0) {
			tVals[i] = (boxMin[i] - capsulePos[i]) / capsuleUp[i];
		}
		else if (posDiff[i] < 0) {
			tVals[i] = (boxMax[i] - capsulePos[i]) / capsuleUp[i];
		}
	}

	float bestT = tVals.x;
	if (abs(tVals.y) < abs(bestT))
		bestT = tVals.y;
	if (abs(tVals.z) < abs(bestT))
		bestT = tVals.z;

	Vector3 intersection = capsulePos + (capsuleUp * bestT);

	Vector3 closestPointOnBox = Clamp(intersection, boxMin, boxMax);

	Vector3 closestPointOnCapsule = ProjectPointOntoLineSegment(capsuleBottom, capsuleTop, closestPointOnBox);


	if (BoxSphereIntersection(boxSize, Vector3(), capsuleRadius, closestPointOnCapsule, collisionInfo,false)) {
		collisionInfo.point.localA = transform * collisionInfo.point.localA;
		collisionInfo.point.localB = transform * (collisionInfo.point.localB + (closestPointOnCapsule - capsulePos));
		collisionInfo.point.normal = transform * collisionInfo.point.normal;
		return true;
	}

	return false;
}