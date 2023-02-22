#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Renderer.h"
#include "Mesh.h"
#include "Texture.h"

#include <GLFW/glfw3.h>

class GameObject {
private:
	typedef struct Instance {
		glm::mat4 model;
		glm::vec3 modelPosition;
		glm::vec3 modelScale;
		glm::vec3 modelRotation;
	};

	std::string ID;
public:
	GLFWwindow* window;

	bool update = false;
	bool fixedUpdate = false;

	std::vector<Instance> modelInstances;

	GameObject(std::string ID, GLFWwindow* window);
	//~GameObject();
	//static void DeleteObject(std::string ID);

	//transforms
	void AddInstance(float posX, float posY, float posZ);
	void SetPosition(unsigned int index, float x, float y, float z);
	void SetRotation(unsigned int index, float x, float y, float z);
	void ScaleModel(unsigned int index, float x, float y, float z);
	glm::mat4 GetModelTransforms(unsigned int index);

	//virtual functions
	virtual void SetAttributes(bool update, bool fixedUpdate) = 0;
	virtual void Update() = 0;
	virtual void FixedUpdate() = 0;
};