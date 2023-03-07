#include "TestPhong.h"

#include "Renderer.h"
#include "imgui/imgui.h"

//#include "tests/lights/CubeLight.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>




namespace test
{
    TestPhong::TestPhong(int wWidth, int wHeight)
        : m_Width(wWidth), m_Height(wHeight),
        m_Proj(glm::perspective(glm::radians(45.0f), (float)m_Width / (float)m_Height, 0.1f, 100.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f))),
        m_Translation(0.0f, 0.0f, 0.0f), m_Rotation(0.0f, 1.0f, 0.0f),
        m_Angle(0.0f), m_lastX(wWidth/2.0f), m_lastY(wHeight/2.0f)
    {
        // lighting
        //glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

        //Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

        float vertexpositions[] = {
            //x, y, z, tex_u, tex_v, n_x, n_y, n_z
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
        };

        const int floatsPerVertex = 8;
        const int numVertices = (sizeof(vertexpositions) / sizeof(float)) / floatsPerVertex;


        unsigned int indices[numVertices];

        for (unsigned int i = 0; i < numVertices; i++) indices[i] = i;

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_DEPTH_TEST));

        m_VAO = std::make_unique<VertexArray>();
        m_VB = std::make_unique<VertexBuffer>(vertexpositions, numVertices * floatsPerVertex * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(3); // x, y, and z
        layout.Push<float>(2); // texture_x and texture_y
        layout.Push<float>(3); // x, y, and z for normal

        m_VAO->AddBuffer(*m_VB, layout);
        m_IB = std::make_unique<IndexBuffer>(indices, numVertices);


        // make the shaders
        m_Shader = std::make_unique<Shader>("res/shaders/TestPhong.shader");
        m_Shader->Bind();
        m_Shader->SetUniform3f("u_lightPos", 1.2f, 1.0f, 2.0f);
        m_Shader->SetUniform3f("u_viewPos", m_camera.Position.x, m_camera.Position.y, m_camera.Position.z);
        m_Shader->SetUniform3f("u_lightColor", 1.0f, 1.0f, 1.0f);
        m_Shader->SetUniform3f("u_objectColor", 1.0f, 0.5f, 0.31f);


        //m_Shader = std::make_unique<Shader>("res/shaders/TestCube.shader");
        //m_Shader->Bind();
        //m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        ////m_CubeLight = CubeLight(m_Height, m_Width, 0.0f, 0.0f, 0.1f);

        //m_Texture = std::make_unique<Texture>("res/textures/box.jpg");
        ////m_Texture = std::make_unique<Texture>("res/textures/sparkle.png");
        //m_Shader->SetUniform1i("u_Texture", 0); // needs to match the slot
    }

    TestPhong::~TestPhong()
    {
    }

    void TestPhong::OnUpdate(float deltaTime)
    {
    }

    void TestPhong::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        Renderer renderer;

        //m_Texture->Bind();

        //{
        //    glm::mat4 model = glm::mat4(1.0f);
        //    model = glm::translate(model, m_Translation); // move model 200 px right and 200 px up
        //    model = glm::rotate(model, m_Angle, m_Rotation);
        //    //glm::mat4 model = translate * rotate;
        //    glm::mat4 mvp = m_Proj * m_View * model;
        //    m_Shader->Bind();
        //    m_Shader->SetUniformMat4f("u_MVP", mvp);
        //    renderer.Draw(*m_VAO, *m_IB, *m_Shader);
        //}


        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, m_Translation); // move model 200 px right and 200 px up
            model = glm::rotate(model, m_Angle, m_Rotation);
            //glm::mat4 model = translate * rotate;
            m_Shader->Bind();
            m_Shader->SetUniform3f("u_viewPos", m_camera.Position.x, m_camera.Position.y, m_camera.Position.z);
            m_Shader->SetUniformMat4f("u_model", model);
            m_Shader->SetUniformMat4f("u_view", m_View);
            m_Shader->SetUniformMat4f("u_projection", m_Proj);

            glm::mat4 mvp = m_Proj * m_View * model;
            renderer.Draw(*m_VAO, *m_IB, *m_Shader);
        }

    }


    void TestPhong::OnImGuiRender()
    {
        //ImGui::SliderFloat3("Translation", &m_Translation.x, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f    
        //ImGui::SliderFloat3("Rotation", &m_Rotation.x, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f    
        //ImGui::SliderFloat("Angle", &m_Angle, 0.0f, 6.283185f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    void TestPhong::ProcessInput(GLFWwindow * window)
    {

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            m_camera.ProcessKeyboard(FORWARD, 0.0f);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            m_camera.ProcessKeyboard(BACKWARD, 0.0f);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            m_camera.ProcessKeyboard(LEFT, 0.0f);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            m_camera.ProcessKeyboard(RIGHT, 0.0f);
    }

    void TestPhong::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
    {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);
        if (m_firstMouse)
        {
            m_lastX = xpos;
            m_lastY = ypos;
            m_firstMouse = false;
        }

        float xoffset = xpos - m_lastX;
        float yoffset = m_lastY - ypos; // reversed since y-coordinates go from bottom to top

        m_lastX = xpos;
        m_lastY = ypos;

        m_camera.ProcessMouseMovement(xoffset, yoffset);
    }




}



