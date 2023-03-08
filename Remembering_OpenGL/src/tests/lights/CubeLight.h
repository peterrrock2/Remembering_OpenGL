#pragma once

#include "Texture.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include <memory>
#include <vector>


class CubeLight
{
public:
	CubeLight(int wWidth, int wHeight, float xpos, float ypos, float zpos);
	CubeLight(int wWidth, int wHeight, glm::vec3 pos);
	//CubeLight(int wWidth, int wHeight);
	~CubeLight();

	void OnUpdate(float deltaTime);
	void OnRender(glm::mat4 proj, glm::mat4 view, glm::mat4 model_position);
	void setTranslation(glm::vec3 trans) { m_Translation = trans; }

private:
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VB;
	std::unique_ptr<IndexBuffer> m_IB;
	std::unique_ptr<Shader> m_Shader;

	int m_Width, m_Height;
	float m_Angle;

	glm::mat4 m_Proj, m_View;
	glm::vec3 m_Translation, m_Rotation;
};
