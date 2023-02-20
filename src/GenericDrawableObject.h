#pragma once

#include "GameObject.h"
#include "Drawable.h"

class GenericDrawableObject : public GameObject, public Drawable {
public:
	GenericDrawableObject(std::string ID, GLFWwindow* window, const char* mesh_path, const char* diffuse_path, const char* specular_path, const char* normal_path);


	void SetAttributes(bool update, bool fixedUpdate) override;
	void Update() override;
	void FixedUpdate() override;
};