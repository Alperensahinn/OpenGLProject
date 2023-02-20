#pragma once

#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
private:
	GLFWwindow* window;
	void UpdateCamera();

	//tmp mouse system vars (from: learnopengl.com)
	bool firstMouse;
	float yaw = -90.0f;
	float pitch = 0.0f;
	float lastX = 1600 / 2;
	float lastY = 900 / 2;

public:
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	Camera(GLFWwindow* window);
	void ProcessMouseInput(float xpos, float ypos);
	void Update(float deltaTime);
};