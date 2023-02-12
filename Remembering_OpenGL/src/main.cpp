#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

#include "Renderer.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestTriangle.h"
#include "tests/TestCube.h"



int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL version 3.*
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL version *.3. Together makes 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    int wWidth = 960;
    int wHeight = 540;
    window = glfwCreateWindow(wWidth, wHeight, "Remembering OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);


    // only draw as fast as vsync
    //glfwSwapInterval(2);

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        // In case the glew libraries fail to be initialized
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return 1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    {

        GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat mat_shininess[] = { 50.0 };
        GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glShadeModel(GL_SMOOTH);

        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_DEPTH_TEST));

        Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();



        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->RegisterTest<test::TestClearColor>("Clear Color");
        testMenu->RegisterTest<test::TestTexture2D>("2D Texture", wWidth, wHeight);
        testMenu->RegisterTest<test::TestTriangle>("Triangle");
        testMenu->RegisterTest<test::TestCube>("Cube", wWidth, wHeight);

        test::TestCube cube(wWidth, wHeight);

    

        while (!glfwWindowShouldClose(window))
        {
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
            /* Render here */
            renderer.Clear();

            // resize the viewport to the window size
            glfwGetWindowSize(window, &wWidth, &wHeight);
            GLCall(glViewport(0, 0, wWidth, wHeight));
          

            glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)wWidth / (float)wHeight, 0.1f, 100.0f);
            glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

            cube.OnRender(proj, view);



            glFlush();

            glfwGetWindowSize(window, &wWidth, &wHeight);

            ImGui_ImplGlfwGL3_NewFrame();
            if (currentTest)
            {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();
                ImGui::Begin("Test");
                if (currentTest != testMenu && ImGui::Button("<-"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->OnImGuiRender();
                ImGui::End();
            }


            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        if (currentTest != testMenu) delete testMenu;
    }


    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}