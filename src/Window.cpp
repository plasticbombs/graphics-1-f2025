#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "Window.h"
#include <cassert>
#include <cstdio>
#include <memory>

struct App
{
	GLFWwindow* window = nullptr;
    int keysPrev[KEY_COUNT]{};
    int keysCurr[KEY_COUNT]{};
} gApp;

void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_REPEAT) return;
    gApp.keysCurr[key] = action;
    
    // Uncomment to see how key events work!
    //const char* name = glfwGetKeyName(key, scancode);
    //if (action == GLFW_PRESS)
    //    printf("%s is down\n", name);
    //else if (action == GLFW_RELEASE)
    //    printf("%s is up\n", name);
}

void CreateWindow(int width, int height, const char* title)
{
    /* Initialize the library */
    assert(glfwInit() == GLFW_TRUE);

    // Request OpenGL 4.3 (1.0 loaded by default)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    gApp.window = glfwCreateWindow(width, height, title, NULL, NULL);
    assert(gApp.window != nullptr);

    /* Make the window's context current */
    glfwMakeContextCurrent(gApp.window);

    // Load OpenGL extensions
    assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));

    glfwSetKeyCallback(gApp.window, KeyboardCallback);
}

void SetWindowShouldClose(bool close)
{
    glfwSetWindowShouldClose(gApp.window, close ? GLFW_TRUE : GLFW_FALSE);
}

bool WindowShouldClose()
{
    return glfwWindowShouldClose(gApp.window);
}

void Loop()
{
    // Last frame escape down
    // This frame escape up
    memcpy(gApp.keysPrev, gApp.keysCurr, sizeof(int) * KEY_COUNT);

    /* Swap front and back buffers */
    glfwSwapBuffers(gApp.window);

    /* Poll for and process events */
    glfwPollEvents();

    //if (IsKeyPressed(KEY_ESCAPE))
    //    glfwSetWindowShouldClose(gApp.window, GLFW_TRUE);
}

bool IsKeyDown(int key)
{
    return gApp.keysCurr[key] == GLFW_PRESS;
}

bool IsKeyUp(int key)
{
    return gApp.keysCurr[key] == GLFW_RELEASE;
}

bool IsKeyPressed(int key)
{
    return 
        gApp.keysPrev[key] == GLFW_PRESS &&
        gApp.keysCurr[key] == GLFW_RELEASE;
}

void DestroyWindow()
{
    glfwTerminate();
}
