#pragma once

#include "Mesh.h"
#include <glad/glad.h>
#include "Texture.h"

//#include "OpenGLError.h"

#include <iostream>

class Renderer {
private:
	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ebo;
	unsigned int diffuse_map;
	unsigned int specular_map;
	unsigned int normal_map;
	unsigned int eboSize;

public:
	Renderer(Mesh mesh, unsigned int diffuse_map_ID, unsigned int specular_map_ID, unsigned int normal_map_ID);
	~Renderer();
	void Draw();
	void Bind();
};