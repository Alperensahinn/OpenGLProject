#pragma once

#include "Mesh.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
private:
	void LoadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	void processMesh(aiMesh* mesh, const aiScene* scene);

public:
	Model(std::string path);
	~Model();
	Mesh* mesh;
};