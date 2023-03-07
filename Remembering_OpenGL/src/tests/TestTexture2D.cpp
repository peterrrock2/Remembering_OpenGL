#include "TestTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//int wWidth = 960;
//int wHeight = 540; // really should get these from application. modify later

namespace test
{
    TestTexture2D::TestTexture2D(int wWidth,int wHeight)
        : m_Width(wWidth), m_Height(wHeight),
          m_Proj(glm::ortho(0.0f, float(wWidth), 0.0f, float(wHeight), -1.0f, 1.0f)), 
          m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
          m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0)
	{
        //float vertexpositions[] = {
        //    // x, y, z, texture_x, texture_y
        //     -50.0f,  -50.0f, 0.0f, 0.0f, 0.0f,
        //      50.0f,  -50.0f, 0.0f, 1.0f, 0.0f,
        //      50.0f,   50.0f, 0.0f, 1.0f, 1.0f,
        //     -50.0f,   50.0f, 0.0f, 0.0f, 1.0f
        //};
        float vertexpositions[] = {
            // x, y, z, texture_x, texture_y
             -50.0f,  -50.0f, 0.0f, 0.0f, 0.0f,
              50.0f,  -50.0f, 0.0f, 1.0f, 0.0f,
              50.0f,   50.0f, 0.0f, 1.0f, 1.0f,
              50.0f,   50.0f, 0.0f, 1.0f, 1.0f,
             -50.0f,   50.0f, 0.0f, 0.0f, 1.0f,
             -50.0f,  -50.0f, 0.0f, 0.0f, 0.0f
        };
        int floatsPerVertex = 5;

        // create an index buffer for the square
        //unsigned int indices[] = {
        //    0,1,2,
        //    2,3,0
        //};
        unsigned int indices[] = {
            0,1,2,
            3,4,5
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_VAO = std::make_unique<VertexArray>();
        //m_VB = std::make_unique<VertexBuffer>(vertexpositions, 4 * floatsPerVertex * sizeof(float));
        m_VB = std::make_unique<VertexBuffer>(vertexpositions, 6 * floatsPerVertex * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(3); // x, y, and z
        layout.Push<float>(2); // texture_x and texture_y

        m_VAO->AddBuffer(*m_VB, layout);
        m_IB = std::make_unique<IndexBuffer>(indices, 6);


        // make the shaders
        m_Shader = std::make_unique<Shader>("res/shaders/TestCube.shader");
        m_Shader->Bind();
        m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        m_Texture = std::make_unique<Texture>("res/textures/sparkle.png");
        m_Shader->SetUniform1i("u_Texture", 0); // needs to match the slot
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        m_Texture->Bind();

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA); // move model 200 px right and 200 px up
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IB, *m_Shader);
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB); // move model 200 px right and 200 px up
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IB, *m_Shader);
        }
	}
	void TestTexture2D::OnImGuiRender()
	{
        ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, float(m_Width));            // Edit 1 float using a slider from 0.0f to 1.0f    
        ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, float(m_Width));            // Edit 1 float using a slider from 0.0f to 1.0f    
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}