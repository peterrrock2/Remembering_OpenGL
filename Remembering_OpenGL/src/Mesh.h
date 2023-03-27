#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Texture.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexStruct.h"

#include <memory>



class Mesh
{
public:
	Mesh(std::vector<VertexStruct> vertices,
		std::vector<unsigned int> indices,
		std::vector<TextureStruct> textures,
		std::vector<std::string> shader_paths);
	~Mesh();

	void OnRender();

private:
	std::unique_ptr<VertexArray> m_VAO; 
	std::unique_ptr<VertexBuffer> m_VB;
	std::unique_ptr<IndexBuffer> m_IB;
	std::vector<std::unique_ptr<Shader>> m_Shaders;
	std::vector<std::unique_ptr<Texture>> m_Textures;
};