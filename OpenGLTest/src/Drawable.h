#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "GameObject.h"

class Drawable {
public:
	typedef struct Material
	{
		Texture* diffuse;
		Texture* specular;
		Texture* normal;
		float shininess;
	};

	Model* model;
	Material* material;
	Renderer* renderer;
public:
	Drawable(const char* mesh_path, const char* diffuse_path, const char* specular_path, const char* normal_path);
	virtual ~Drawable() = 0;
	void Bind();
	void Draw();
private:
	void deleteAllocateds();
};