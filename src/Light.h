#pragma once

#include "GameObject.h"

class Light : public GameObject 
{
public:
	struct Attrib {
		glm::vec3 direction;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};
	struct Attrib attrib;

	Light(std::string ID, GLFWwindow* window);

	void SetAttributes(bool update, bool fixedUpdate) override;
	void Update() override;
	void FixedUpdate() override;
};