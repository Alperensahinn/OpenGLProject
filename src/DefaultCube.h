#pragma once

#include "GameObject.h"
#include "Drawable.h"

class DefaultCube : public GameObject, public Drawable{
public: 
	DefaultCube(std::string ID, GLFWwindow* window);


	void SetAttributes(bool update, bool fixedUpdate) override;
	void Update() override;
	void FixedUpdate() override;
};