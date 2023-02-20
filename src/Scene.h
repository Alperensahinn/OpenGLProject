#pragma once

#include <vector>
#include "GameObject.h"
#include "Shader.h"
#include "Drawable.h"
#include <glm/glm.hpp>
#include "GenericDrawableObject.h"
#include "Light.h"
#include "Camera.h"
#include "DefaultCube.h"

class Scene {
private:
	//void shadowBuffer();
public:
	std::vector<GameObject*> gm;
	Light* light;
	Camera* camera;
public:
	Scene(GLFWwindow* window, Shader shader);
	~Scene();
	void DrawScene(Shader shader, glm::mat4 projection, glm::mat4 view, glm::mat4 lightSpaceMatrix, unsigned int depthMap);
};