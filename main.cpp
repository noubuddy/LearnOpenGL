#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main()
{
    const int width = 1000;
    const int height = 1000;
    
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

    /*-----------------------------------------------MAIN LOOP-------------------------------------------------------*/
    // keep the window opened / main loop
    while (!glfwWindowShouldClose(window))
    {
        // specify the color of the background
        glClearColor(0.29f, 0.66f, 0.87f, 1.0f);
        // clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
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
