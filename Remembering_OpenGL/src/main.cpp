#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <array>

#include "Renderer.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "util/perspCamera.h"


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestTriangle.h"
#include "tests/TestCube.h"
#include "tests/TestPhong.h"
#include "tests/TestLights.h"

#include "atomics/Cube.h"


#include <GL/gl.h>
#include <GL/glu.h>


// Move these into abstracted places later
void processInput(GLFWwindow* window, Camera& camera, float deltaTime, bool& mouseMove);
void mouse_callback(GLFWwindow* window, Camera& camera, float& lastX, float& lastY);

void registerTests(test::TestMenu* menu, GLFWwindow* window, int wWidth, int wHeight);


int main(void)
{
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;

    bool mouseMove = false;


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


    float lastX = wWidth / 2.0f;
    float lastY = wHeight / 2.0f;

    window = glfwCreateWindow(wWidth, wHeight, "Remembering OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, NULL);

    glewInit();


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);//capture the mouse

    glm::vec3 tester(1.0f);
    std::cout << "Size of test " << sizeof(tester)/sizeof(float) << std::endl;

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
        Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

        glClearColor(0.0, 0.0, 0.0, 0.0);
        glShadeModel(GL_SMOOTH);
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_DEPTH_TEST));

        Renderer renderer;
        
        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();


        test::TestLights myLights(window, wWidth, wHeight);


        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        registerTests(testMenu, window, wWidth, wHeight);



        while (!glfwWindowShouldClose(window))
        {
            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = 1.5f*(currentFrame - lastFrame);
            lastFrame = currentFrame;

            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
            /* Render here */
            renderer.Clear();

            // resize the viewport to the window size
            glfwGetWindowSize(window, &wWidth, &wHeight);
            GLCall(glViewport(0, 0, wWidth, wHeight));


            processInput(window, camera, deltaTime, mouseMove);
            if(mouseMove)
                mouse_callback(window, camera, lastX, lastY);
      

            glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)wWidth / (float)wHeight, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            
            myLights.OnRender();

            glFlush();

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










void processInput(GLFWwindow* window, Camera& camera, float deltaTime, bool& mouseMove)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        mouseMove = false;
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        mouseMove = true;
    }

}


void mouse_callback(GLFWwindow* window, Camera& camera, float& lastX, float& lastY)
{
    GLdouble xPosIn, yPosIn;
    glfwGetCursorPos(window, &xPosIn, &yPosIn);


    float xpos = static_cast<float>(xPosIn);
    float ypos = static_cast<float>(yPosIn);

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void registerTests(test::TestMenu* menu, GLFWwindow* window, int wWidth, int wHeight)
{
    menu->RegisterTest<test::TestClearColor>("Clear Color");
    menu->RegisterTest<test::TestTexture2D>("2D Texture", wWidth, wHeight);
    menu->RegisterTest<test::TestTriangle>("Triangle");
    menu->RegisterTest<test::TestCube>("Cube", wWidth, wHeight);
    menu->RegisterTest<test::TestPhong>("Phong", window, wWidth, wHeight);
    menu->RegisterTest<test::TestLights>("Lights", window, wWidth, wHeight);
}