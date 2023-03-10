#include "TestPhong.h"

#include "Renderer.h"
#include "imgui/imgui.h"
#include <cmath>



#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

//for debugging
#include <glm/gtx/string_cast.hpp>




namespace test
{
    TestPhong::TestPhong(GLFWwindow* window, int wWidth, int wHeight)
      : m_Width(wWidth), m_Height(wHeight),
        m_Proj(glm::perspective(glm::radians(45.0f), (float)m_Width / (float)m_Height, 0.1f, 100.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f))),
        m_Translation(0.0f, 0.0f, 0.0f), m_Rotation(0.0f, 1.0f, 0.0f),
        m_Angle(0.0f), m_lastX(wWidth / 2.0f), m_lastY(wHeight / 2.0f),
        m_window(window),
        m_deltaTime(0.0f), m_currentFrame(0.0f), m_lastFrame(0.0f),
        m_cubeLight(wWidth, wHeight, 1.2f, 1.0f, 2.0f)
    {
        // lighting
        m_LightPos = glm::vec3(cos(m_time), 0.6f, sin(m_time));
        m_cubeLight.setTranslation(m_LightPos);
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


        m_Diffuse = std::make_unique<Texture>("res/textures/container_diffuse.png");
        m_Specular = std::make_unique<Texture>("res/textures/container_specular.png");
        m_Shader->SetUniform1i("material.diffuse", 0);
        m_Shader->SetUniform1i("material.specular", 1);

        //set the material properties
        m_Shader->SetUniform1f("material.shininess", 64.0f);

        //set the static light properties
        m_Shader->SetUniform3f("light.ambient", m_cubeLight.m_ambient);
        m_Shader->SetUniform3f("light.diffuse", m_cubeLight.m_diffuse);
        m_Shader->SetUniform3f("light.specular", glm::vec3(1.0f));
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
        GLCall(glEnable(GL_DEPTH_TEST));

        Renderer renderer;

        ProcessInput(m_window);
        if (m_mouseControl) mouse_callback();
        m_currentFrame = static_cast<float>(glfwGetTime());
        m_deltaTime = m_currentFrame - m_lastFrame;
        m_lastFrame = m_currentFrame;
        

        if(m_moveBool)
        {
            m_time = (m_time + m_deltaTime);
            if (m_time >= 2 * PI)
                m_time -= 2 * PI;
        }

        m_LightPos.x = cos(m_time);
        m_LightPos.z = sin(m_time);
        m_cubeLight.setTranslation(m_LightPos);


        m_Diffuse->Bind(0);
        m_Specular->Bind(1);




        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, m_Translation);
            model = glm::rotate(model, m_Angle, m_Rotation);
            m_Shader->Bind();


            //set the light properties
            m_Shader->SetUniform3f("light.position", m_LightPos);

            m_Shader->SetUniform3f("u_viewPos", m_camera.m_Position);
            m_Shader->SetUniformMat4f("u_model", model);

            m_View = m_camera.GetViewMatrix();
            m_Shader->SetUniformMat4f("u_view", m_View);
            m_Shader->SetUniformMat4f("u_projection", m_Proj);

            m_cubeLight.OnRender(m_Proj, m_View, glm::translate(glm::mat4(1.0f), m_LightPos));


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
            m_camera.ProcessKeyboard(FORWARD, m_deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            m_camera.ProcessKeyboard(BACKWARD, m_deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            m_camera.ProcessKeyboard(LEFT, m_deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            m_camera.ProcessKeyboard(RIGHT, m_deltaTime);
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            m_camera.ProcessKeyboard(UP, m_deltaTime);
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
            m_camera.ProcessKeyboard(DOWN, m_deltaTime);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            m_mouseControl = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        {
            m_mouseControl = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
            m_moveBool = !m_moveBool;
    }

    void TestPhong::mouse_callback()
    {
        GLdouble xPosIn, yPosIn;
        glfwGetCursorPos(m_window, &xPosIn, &yPosIn);


        float xpos = static_cast<float>(xPosIn);
        float ypos = static_cast<float>(yPosIn);
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



