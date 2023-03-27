#pragma once

#include "glm/glm.hpp"
#include <string>


// will be using this to help us load the models
// will also make constructing the vbo much easier

struct VertexStruct
{
	glm::vec3 position;
	glm::vec2 texcoord;
	glm::vec3 normal;
};


struct TextureStruct
{
	unsigned int id;
	std::string type;
};