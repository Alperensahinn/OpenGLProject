#include "Light.h"

Light::Light(std::string ID, GLFWwindow* window) : GameObject(ID, window)
{
	attrib.direction = glm::vec3(-8.0f, 16.0f, -4.0f);
	attrib.ambient = glm::vec3(0.25f, 0.3f, 0.35f);
	attrib.diffuse = glm::vec3(0.9f, 0.9f, 0.9f);
	attrib.specular = glm::vec3(0.8f, 0.8f, 0.8f);
}

void Light::SetAttributes(bool update, bool fixedUpdate)
{
}

void Light::Update()
{
}

void Light::FixedUpdate()
{
}
