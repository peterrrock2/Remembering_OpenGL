#pragma once
#include "Test.h"

#include "Texture.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include <memory>

#include "glm/glm.hpp"
#include "util/orthoCamera.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace test
{
	class TestPhong : public Test
	{
	public:
		TestPhong(GLFWwindow* window, int wWidth, int wHeight);
		~TestPhong();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void ProcessInput(GLFWwindow* window);
		void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VB;
		std::unique_ptr<IndexBuffer> m_IB;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;



		int m_Width, m_Height;
		float m_Angle;
		float m_lastX, m_lastY;
		bool m_firstMouse = true;
		// just for the test Phong class
		Camera m_camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
		GLFWwindow* m_window;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_Translation, m_Rotation;
	};


}