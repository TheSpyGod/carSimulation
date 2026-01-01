#include <iostream>
#include "include/GLFW/glfw3.h"

int main() {
    std::cout << 2;
    if (!glfwInit()) {
        return -1;
    } 
    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window)) {
            glClear(GL_COLOR_BUFFER_BIT);
            glfwSwapBuffers(window);
            glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}