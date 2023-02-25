#include "RigidBody.h"

void RigidBody::CreateRigedBody(RBDesc rbDesc, GameObject* gm)
{
	if(rbDesc.shape == 1)
	{
		physx::PxTriangleMesh* trisMesh = this->CreateTriangleMesh(rbDesc, gm);

		if(rbDesc.type == 0)
		{
			this->rb = rbDesc.mPhysics->createRigidStatic(physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)));
			physx::PxShape* meshShape;
			physx::PxTriangleMeshGeometry triGeom;
			triGeom.triangleMesh = trisMesh;
			meshShape = physx::PxRigidActorExt::createExclusiveShape(*this->rb, triGeom,
				*rbDesc.mMaterial);

			if (this->rb == nullptr) 
			{
				std::cout << "PHYSX ERROR!!: create meshActor failed!" << std::endl;
			}

			rbDesc.mScene->addActor(*this->rb);
		}
	}
}

physx::PxTriangleMesh* RigidBody::CreateTriangleMesh(RBDesc rbDesc, GameObject* gm)
{
	std::vector<physx::PxVec3> cVerts;
	std::vector<physx::PxU32> cInds;

	Drawable* drw = dynamic_cast<Drawable*>(gm);

	for (int i = 0; i < drw->model->mesh->vertices.size(); i++)
	{
		cVerts.push_back(physx::PxVec3(drw->model->mesh->vertices[i].Position.x, drw->model->mesh->vertices[i].Position.y, drw->model->mesh->vertices[i].Position.z));
	}

	for (int i = 0; i < drw->model->mesh->indices.size(); i++)
	{
		cInds.push_back(drw->model->mesh->indices[i]);
	}

	physx::PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = cVerts.size();
	meshDesc.points.stride = sizeof(physx::PxVec3);
	meshDesc.points.data = &cVerts[0];

	meshDesc.triangles.count = cInds.size() / 3;
	meshDesc.triangles.stride = 3 * sizeof(physx::PxU32);
	meshDesc.triangles.data = &cInds[0];

	physx::PxDefaultMemoryOutputStream writeBuffer;
	physx::PxTriangleMeshCookingResult::Enum result;
	bool status = rbDesc.mCooking->cookTriangleMesh(meshDesc, writeBuffer, &result);

	if (!rbDesc.mCooking->cookTriangleMesh(meshDesc, writeBuffer, &result)) 
	{
		std::cout << "PHYSX ERROR!!: cookTriangleMesh failed!" << std::endl;
		return nullptr;
	}

	physx::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	return rbDesc.mPhysics->createTriangleMesh(readBuffer);
}
