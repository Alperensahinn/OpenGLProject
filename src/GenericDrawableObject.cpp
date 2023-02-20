#include "GenericDrawableObject.h"

GenericDrawableObject::GenericDrawableObject(std::string ID, GLFWwindow* window, const char* mesh_path, const char* diffuse_path, const char* specular_path, const char* normal_path)
	: GameObject(ID, window), Drawable(mesh_path, diffuse_path, specular_path, normal_path)
{
}

void GenericDrawableObject::SetAttributes(bool update, bool fixedUpdate)
{
	this->update = update;
	this->fixedUpdate = fixedUpdate;
}

void GenericDrawableObject::Update()
{
	
}

void GenericDrawableObject::FixedUpdate()
{
	
}
