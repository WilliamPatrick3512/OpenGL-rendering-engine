#include <Windows.h>
// Include standard headers
#include <cstdio>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

// Local / project headers
#include "../Common/Scene.h"
#include "shader.hpp"
#include "SimpleScene_Quad.h"
#include "../CS300 Project/cs300gui.h"

//////////////////////////////////////////////////////////////////////

GLFWwindow *window;
GLFWwindow *debug_window;
Scene  *scene;

int windowWidth = 1024;
int windowHeight = 768;

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
int main()
{
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    // Setting up OpenGL properties
    glfwWindowHint(GLFW_SAMPLES, 1); // change for anti-aliasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(windowWidth, windowHeight, // window dimensions
                              "Deffered Shading Scene", // window title
                              nullptr, // which monitor (if full-screen mode)
                              nullptr); // if sharing context with another window

    if (window == nullptr)
    {
        fprintf(stderr,
                "Failed to open GLFW window. Check if your GPU is 4.0 compatible.\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // OpenGL resource model - "glfwCreateWindow" creates the necessary data storage for the OpenGL
    // context but does NOT make the created context "current". We MUST make it current with the following
    // call
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = static_cast<GLboolean>(true); // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Initialize the scene
    scene = new SimpleScene_Quad( windowWidth, windowHeight );

    // Scene::Init encapsulates setting up the geometry and the texture
    // information for the scene
    scene->Init();
	initializeGUI(window);
    do
    {
        // Now render the scene
        // Scene::Display method encapsulates pre-, render, and post- rendering operations
        scene->Display();
		scene->CheckInputs(window);
		
		updateGUI();
		GUIsettings();
		renderGUI();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);
	
    // Close OpenGL window and terminate GLFW with gui
    glfwTerminate();
	shutdownGUI();

    return 0;
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////