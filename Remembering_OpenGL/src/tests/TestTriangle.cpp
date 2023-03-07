#include "TestTriangle.h"

#include "Renderer.h"
#include "imgui/imgui.h"

namespace test
{
    TestTriangle::TestTriangle()
	{
        float vertexpositions[] = {
            // x, y, r, g, b, a
             -0.5f,  -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
              0.0f,   0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
              0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        };


        int numberVertices = 3;
        int floatsPerVertex = sizeof(vertexpositions) / (numberVertices * sizeof(float));


        // create an index buffer for the triangle
        unsigned int indices[] = {
            1,2,0
        };

        int numIndices = sizeof(indices) / sizeof(unsigned int);

        m_VAO = std::make_unique<VertexArray>();
        m_VB = std::make_unique<VertexBuffer>(vertexpositions, numberVertices * floatsPerVertex * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2); // xy 
        layout.Push<float>(floatsPerVertex - 2); // rgb

        m_VAO->AddBuffer(*m_VB, layout);
        m_IB = std::make_unique<IndexBuffer>(indices, numIndices);


        // make the shaders
        m_Shader = std::make_unique<Shader>("res/shaders/TestTriangle.shader");
        m_Shader->Bind();
        //m_Shader->SetUniform4f("u_Color", 0.0f, 0.0f, 1.0f, 1.0f);
	}



	TestTriangle::~TestTriangle()
	{
	}

	void TestTriangle::OnUpdate(float deltaTime)
	{
	}

	void TestTriangle::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        //m_Shader->Bind();

        renderer.Draw(*m_VAO, *m_IB, *m_Shader);
	}
	void TestTriangle::OnImGuiRender()
	{
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}