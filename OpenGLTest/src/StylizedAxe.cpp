#include "StylizedAxe.h"

StylizedAxe::StylizedAxe(std::string ID, GLFWwindow* window) 
	: GameObject(ID, window), Drawable("Resources/Axe_Lowpoly.obj", "Resources/Axe_Lowpoly_Axe_BaseColor.png", "Resources/WoodenPlanks_Specular.png", "Resources/Axe_Lowpoly_Axe_Normal.png")
{

}

void StylizedAxe::SetAttributes(bool update, bool fixedUpdate)
{
	this->update = update;
	this->fixedUpdate = fixedUpdate;
}

void StylizedAxe::Update()
{

}

void StylizedAxe::FixedUpdate()
{
}