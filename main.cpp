#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define TRI(x, y, z) x, y, z,

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "} ";

int main()
{
    const int width = 1000;
    const int height = 1000;

    float vertices[] = {
        TRI(-0.5f, -0.5f, 0.0f)
        TRI(0.5f, -0.5f, 0.0f)
        TRI(0.0f, 0.5f, 0.0f)
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
    
    std::cout << "Compiling shaders..." << std::endl;
    
    // init vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // specify source for vertex shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // compile vertex shader
    glCompileShader(vertexShader);

    // init fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // specify source for fragment shader
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // compile fragment shader
    glCompileShader(fragmentShader);

    // init shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    // link shaders with shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    std::cout << "Shader compilation finished!" << std::endl;

    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);  
    
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    
    // copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    

    // now draw the object 
    
    /*-----------------------------------------------MAIN LOOP-------------------------------------------------------*/
    // keep the window opened / main loop
    while (!glfwWindowShouldClose(window))
    {
        // specify the color of the background
        glClearColor(0.29f, 0.66f, 0.87f, 1.0f);
        // clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // use our shader program when we want to render an object (draw our first triangle)
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        
        // swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // process all the pooled events / make the window responding / take care of all GLFW event
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    /*--------------------------------------------------TERMINATION--------------------------------------------------*/
    
    // destroy window before ending the program
    glfwDestroyWindow(window);

    // terminate glfw before ending the program
    glfwTerminate();

    return 0;
}
