#include "Drawable.h"

Drawable::Drawable(const char* mesh_path, const char* diffuse_path, const char* specular_path, const char* normal_path)
{
	model = new Model(mesh_path);
	material = new Material();
	material->diffuse = new Texture(diffuse_path);
	material->specular = new Texture(specular_path);
	material->normal = new Texture(normal_path);
	material->shininess = 32.0f;

	renderer = new Renderer(*(model->mesh), this->material->diffuse->ID, this->material->specular->ID, this->material->normal->ID);
	this->deleteAllocateds();
}

Drawable::~Drawable()
{
	delete renderer;
}

void Drawable::Bind()
{
	this->renderer->Bind();
}

void Drawable::Draw()
{
	this->renderer->Draw();
}

void Drawable::deleteAllocateds()
{
	delete this->model;
	delete this->material->diffuse;
	delete this->material->specular;
	delete this->material->normal;
	delete this->material;
}
