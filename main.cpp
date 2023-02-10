#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

#define VERT(x, y, z) x, y, z,

int main()
{
    const int width = 1000;
    const int height = 1000;

    float vertices[] = {
        VERT(-0.5f, -0.5f, 0.0f)
        VERT(0.5f, -0.5f, 0.0f)
        VERT(0.0f, 0.5f, 0.0f)
    };

    /*--------------------------------------------INITIALIZATION----------------------------------------------------*/
    // init glfw
    glfwInit();
    // tell glfw what version of OpenGL we are using (3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // tell glfw we are using the CORE profile so we only have the modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /*---------------------------------------------CREATE WINDOW----------------------------------------------------*/
    // create window
    GLFWwindow* window = glfwCreateWindow(width, height, "Window", nullptr, nullptr);
    // check an issues while creating the window
    if (window == nullptr)
    {
        std::cout << "Failed to create window \n";
        glfwTerminate();
        return -1;
    }

    // introduce the window into the current context
    glfwMakeContextCurrent(window);
    // load GLAD so it configures OpenGL
    gladLoadGL();
    // specify the viewport of OpenGl in the window
    glViewport(0, 0, width, height);

    /* Init and compile shaders here */

    Shader shader_program("default.vert", "default.frag");

    /* Create and configure buffers here */

    // 0. declare a variable to store vao
    unsigned int vao;
    // 1. generate buffer
    glGenVertexArrays(1, &vao);
    // 2. bind buffer to select it
    glBindVertexArray(vao);

    // 0. declare a variable to store vbo
    unsigned int vbo;
    // 1. generate buffer
    glGenBuffers(1, &vbo);
    // 2. bind buffer to select it
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // 3. put some data to the selected buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, vertices, GL_STATIC_DRAW);
    // 4. specify how opengl will interpret vertex data in selected buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // 5. enable vertex attributes
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    /*-----------------------------------------------MAIN LOOP-------------------------------------------------------*/
    // keep the window opened / main loop
    while (!glfwWindowShouldClose(window))
    {
        // specify the color of the background
        glClearColor(0.29f, 0.66f, 0.87f, 1.0f);
        // clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Draw calls here */

        // 1. specify what shader program we want to use
        shader_program.Activate();
        // 2. bind vao so opengl knows to use it
        glBindVertexArray(vao);
        // 3. draw triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // process all the pooled events / make the window responding / take care of all GLFW event
        glfwPollEvents();
    }
    

    /*--------------------------------------------------TERMINATION--------------------------------------------------*/
    
    // destroy window before ending the program
    glfwDestroyWindow(window);

    // terminate glfw before ending the program
    glfwTerminate();

    return 0;
}
