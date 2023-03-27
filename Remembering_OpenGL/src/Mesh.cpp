#include "Mesh.h"

Mesh::Mesh(std::vector<VertexStruct> vertices, 
	       std::vector<unsigned int> indices, 
	       std::vector<TextureStruct> textures,
		   std::vector<std::string> shader_paths)
{

	// now to set everything up
	const int floatsPerVertex = sizeof(VertexStruct) / sizeof(float);
	const int numVertices = sizeof(vertices) / sizeof(VertexStruct);
	

	m_VAO = std::make_unique<VertexArray>();
	m_VB = std::make_unique<VertexBuffer>(vertices, numVertices * floatsPerVertex * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(3); // VertexStruct position
	layout.Push<float>(2); // VertexStruct texcoord
	layout.Push<float>(3); // VertexStruct normal

	m_VAO->AddBuffer(*m_VB, layout);
	m_IB = std::make_unique<IndexBuffer>(indices, numVertices);

	// do something with the shaders
	for (auto path : shader_paths)
		m_Shaders.push_back(std::make_unique<Shader>(path));

	// now deal with lots of textures somehow

}

Mesh::~Mesh() {}

void Mesh::OnRender()
{

}