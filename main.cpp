#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "Camera.h"
#include "PerlinNoise.h"

GLfloat vertices[] =
{
    // Front Face
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom-left
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom-right
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top-right
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Top-left

    // Back Face
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom-right
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom-left
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Top-left
    -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top-right

    // Top Face
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom-left
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom-right
    -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Top-right
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top-left

    // Bottom Face
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom-right
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom-left
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Top-left
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top-right

    // Right face
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

    // Left face
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
};

GLuint indices[] =
{
    // Front Face
    0, 1, 2,
    0, 3, 2,
    // Back Face
    4, 5, 6,
    4, 7, 6,
    // Top face
    8, 9, 10,
    8, 11, 10,
    // Bottom Face
    12, 13, 14,
    12, 15, 14,
    // Right Face
    16, 17, 18,
    16, 19, 18,
    // Left Face
    20, 21, 22,
    20, 23, 22,
};

std::vector<glm::vec3> cubePositions
    = {
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(2.0f, 1.0f, 0.0f),
        glm::vec3(3.0f, 1.0f, 0.0f),
        glm::vec3(4.0f, 1.0f, 0.0f),
        glm::vec3(5.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(2.0f, 1.0f, 1.0f),
        glm::vec3(3.0f, 1.0f, 1.0f),
        glm::vec3(4.0f, 1.0f, 1.0f),
        glm::vec3(5.0f, 1.0f, 1.0f)
    };


int main()
{
    const int width = 1300;
    const int height = 1300;

    // Constants for the grid size and cube size
    // const int SIZE = 100;
    const float CUBE_SIZE = 1;
    const float THRESHOLD = 0.5;

    // Constants for the noise parameters
    const float NOISE_SCALE = 0.05;
    // const int OCTAVES = 4;
    // const float PERSISTENCE = 0.5;

    // Initialize the Perlin noise object
    PerlinNoise noise;
    // PerlinNoise extra_noise;

    std::vector<std::vector<float>> noiseValues(GRID_SIZE, std::vector<float>(GRID_SIZE));
    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            for (int z = 0; z < GRID_SIZE; ++z)
            {
                float noiseValue = noise.noise(x * NOISE_SCALE, y * NOISE_SCALE, z * NOISE_SCALE);
                noiseValues[y][x] = noiseValue;
            }
        }
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Window", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create window \n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glViewport(0, 0, width, height);

    Shader shader_program("default.vert", "default.frag");

    VAO vao1;
    vao1.Bind();

    VBO vbo1(vertices, sizeof(vertices));
    EBO ebo1(indices, sizeof(indices));

    vao1.LinkAttrib(vbo1, 0, 3, GL_FLOAT, 8 * sizeof(float), nullptr);
    vao1.LinkAttrib(vbo1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    vao1.LinkAttrib(vbo1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    vao1.Unbind();
    vbo1.Unbind();
    ebo1.Unbind();

    Texture texture("block-top.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    texture.TexUnit(shader_program, "tex0", 0);

    // Texture texture2("block-top.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    // texture2.TexUnit(shader_program, "tex0", 0);

    glEnable(GL_DEPTH_TEST);

    Camera camera(width, height, glm::vec3(0.0f, 0.5f, 5.0f));

    shader_program.Activate();

    double lastTime = glfwGetTime();
    int nbFrames = 0;

    while (!glfwWindowShouldClose(window))
    {
        // FPS couonter
        const double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0)
        {
            std::cout << 1000.0 / double(nbFrames) << std::endl;
            nbFrames = 0;
            lastTime += 1.0;
        }

        glClearColor(0.29f, 0.66f, 0.87f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.speed = 0.01f;
        camera.Inputs(window);

        texture.Bind();
        vao1.Bind();

        // for (auto& cubePosition : cubePositions)
        // {
        //     glm::mat4 model = glm::mat4(1.0f);
        //     model = glm::translate(model, cubePosition);
        //
        //     camera.Matrix(80.0f, 0.1f, 100.0f, shader_program, "camMatrix", model);
        //     glDrawElements(GL_TRIANGLES, sizeof indices / sizeof(int), GL_UNSIGNED_INT, nullptr);
        // }   

        for (int y = 0; y < GRID_SIZE; y++)
        {
            for (int x = 0; x < GRID_SIZE; x++)
            {
                // for (int z = 0; z < GRID_SIZE; z++)
                // {
                    if (noiseValues[y][x] > THRESHOLD)
                    {
                        glm::mat4 model = glm::mat4(1.0f);
                        model = glm::translate(glm::mat4(1.0f),
                                               glm::vec3(x * CUBE_SIZE, noiseValues[y][x] * CUBE_SIZE, y * CUBE_SIZE));

                        // GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
                        // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

                        camera.Matrix(80.0f, 0.1f, 100.0f, shader_program, "camMatrix", model);
                        glDrawElements(GL_TRIANGLES, sizeof indices / sizeof(int), GL_UNSIGNED_INT, nullptr);
                        // glDrawArrays(GL_TRIANGLES, 0, 36);
                    }
                // }
            }
        }

        // texture2.Bind();
        //
        // for (int i = 0; i < cubePositions.size(); i++)
        // {
        //     glm::mat4 model = glm::mat4(1.0f);
        //     model = glm::translate(model, cubePositions[i]);
        //     
        //     camera.Matrix(80.0f, 0.1f, 100.0f, shader_program, "camMatrix", model);
        //     glDrawArrays(GL_TRIANGLES, 8, 4);
        // }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    vao1.Delete();
    vbo1.Delete();
    ebo1.Delete();
    texture.Delete();
    shader_program.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
