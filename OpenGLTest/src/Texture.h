#pragma once

#include <glad/glad.h>
#include "ThirdParty/stb_image.h"
#include <iostream>

class Texture {
public:
	unsigned int ID;
	Texture(const char* filename);
	~Texture();
};