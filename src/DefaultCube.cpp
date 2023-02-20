#include "DefaultCube.h"
DefaultCube::DefaultCube(std::string ID, GLFWwindow* window) 
	: GameObject(ID, window), Drawable("Resources/Cube_2x2x2m.glb", "Resources/stone_wall_diff_2k.jpg", "Resources/GenericSpecularMap_1K.jpg", "Resources/stone_wall_nor_gl_2k.jpg")
{
    
}

void DefaultCube::SetAttributes(bool update, bool fixedUpdate)
{
	this->update = update;
	this->fixedUpdate = fixedUpdate;
}

void DefaultCube::Update()
{

}

void DefaultCube::FixedUpdate()
{
}
