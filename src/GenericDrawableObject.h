#pragma once

#include "GameObject.h"
#include "Drawable.h"
#include "RigidBody.h"

class GenericDrawableObject : public GameObject, public Drawable{
public:

	RigidBody* rb;

	GenericDrawableObject(std::string ID, GLFWwindow* window, const char* mesh_path, const char* diffuse_path, const char* specular_path, const char* normal_path);
	~GenericDrawableObject();

	void SetAttributes(bool update, bool fixedUpdate) override;
	void AddRigidBody(RBDesc rbDesc);
	void Update() override;
	void FixedUpdate() override;
};