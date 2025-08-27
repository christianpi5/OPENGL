#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "LibCalls.h"


GLFWwindow* LibCalls()
{
    GLFWwindow* window;

    if (!glfwInit())
        return nullptr;

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        std::cout << "Error!\n";
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!\n"; //precisa vir depois de tornar context current
        return nullptr;
    }

    return window;
}