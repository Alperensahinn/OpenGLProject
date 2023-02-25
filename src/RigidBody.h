#pragma once

#include <PxPhysicsAPI.h>
#include "GameObject.h"
#include "Drawable.h"

typedef struct RBDesc 
{
	physx::PxPhysics* mPhysics;
	physx::PxCooking* mCooking;
	physx::PxScene* mScene;

	physx::PxMaterial* mMaterial;

	int type; //0 for static 1 for dynamic
	int shape; //0 for box 1 for triangle 2 for convex

	float hX; 
	float hY; 
	float hZ;

	float staticFriction = 0.5f; 
	float dynamicFriction = 0.5f;
	float restitution = 0.05f;

	float denstiy;
};

class RigidBody {
private:
	physx::PxRigidStatic* rb;
public:
	void CreateRigedBody(RBDesc rbDesc, GameObject* gm);
	physx::PxTriangleMesh* CreateTriangleMesh(RBDesc rbDesc, GameObject* gm);
};