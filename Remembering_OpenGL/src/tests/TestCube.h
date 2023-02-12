#pragma once

#include "Test.h"

#include "Texture.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include <memory>

namespace test
{
	class TestCube : public Test
	{
	public:
		TestCube(int wWidth, int wHeight);
		~TestCube();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnRender(glm::mat4 proj, glm::mat4 view);
		void OnImGuiRender() override;

	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VB;
		std::unique_ptr<IndexBuffer> m_IB;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		int m_Width, m_Height;
		float m_Angle;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_Translation, m_Rotation;
	};


}