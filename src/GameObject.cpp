#include "GameObject.h"

GameObject::GameObject(std::string ID, GLFWwindow* window)
{
	this->ID = ID;
	this->window = window;
}

void GameObject::AddInstance(float posX, float posY, float posZ)
{
	Instance instance;
	instance.modelPosition = glm::vec3(posX, posY, posZ);
	instance.modelScale = glm::vec3(1, 1, 1);
	instance.modelRotation = glm::vec3(0, 0, 0);
	instance.model = glm::translate(glm::mat4(1.0f), glm::vec3(posX, posY, posZ));
	this->modelInstances.push_back(instance);
}

void GameObject::SetPosition(unsigned int index, float x, float y, float z)
{	
	this->modelInstances[index].modelPosition = glm::vec3(x, y, z);

	glm::mat4 tmp = glm::mat4(1.0f);
	tmp = glm::translate(tmp, this->modelInstances[index].modelPosition);

	glm::quat quat = glm::quat(glm::vec3(
		glm::radians(this->modelInstances[index].modelRotation.x),
		glm::radians(this->modelInstances[index].modelRotation.y), 
		glm::radians(this->modelInstances[index].modelRotation.z)));
	glm::mat4 rotation = glm::toMat4(quat);
	tmp = tmp * rotation;

	tmp = glm::scale(tmp, this->modelInstances[index].modelScale);

	this->modelInstances[index].model = tmp;
}

void GameObject::SetRotation(unsigned int index, float x, float y, float z)
{
	this->modelInstances[index].modelRotation = glm::vec3(x, y, z);

	glm::mat4 tmp = glm::mat4(1.0f);
	tmp = glm::translate(tmp, this->modelInstances[index].modelPosition);

	glm::quat quat = glm::quat(glm::vec3(
		glm::radians(this->modelInstances[index].modelRotation.x),
		glm::radians(this->modelInstances[index].modelRotation.y),
		glm::radians(this->modelInstances[index].modelRotation.z)));
	glm::mat4 rotation = glm::toMat4(quat);
	tmp = tmp * rotation;

	tmp = glm::scale(tmp, this->modelInstances[index].modelScale);

	this->modelInstances[index].model = tmp;
}

void GameObject::ScaleModel(unsigned int index, float x, float y, float z)
{
	this->modelInstances[index].modelScale = glm::vec3(x, y, z);

	glm::mat4 tmp = glm::mat4(1.0f);
	tmp = glm::translate(tmp, this->modelInstances[index].modelPosition);

	glm::quat quat = glm::quat(glm::vec3(
		glm::radians(this->modelInstances[index].modelRotation.x),
		glm::radians(this->modelInstances[index].modelRotation.y),
		glm::radians(this->modelInstances[index].modelRotation.z)));
	glm::mat4 rotation = glm::toMat4(quat);
	tmp = tmp * rotation;

	tmp = glm::scale(tmp, this->modelInstances[index].modelScale);

	this->modelInstances[index].model = tmp;
}

glm::mat4 GameObject::GetModelTransforms(unsigned int index)
{
	return this->modelInstances[index].model;
}
