#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
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
// #include "Vertices.inl"
#include "Vertices.h"


// GLfloat vertices[] =
// {
//     // Front Face
//     -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom-left
//     0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom-right
//     0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top-right
//     -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Top-left
//
//     // Back Face
//     -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom-right
//     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom-left
//     0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Top-left
//     -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top-right
//
//     // Top Face
//     0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom-left
//     -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom-right
//     -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Top-right
//     0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top-left
//
//     // Bottom Face
//     -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom-right
//     0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom-left
//     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Top-left
//     -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top-right
//
//     // Right face
//     0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
//     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
//     0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
//     0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
//
//     // Left face
//     -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
//     -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
//     -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
//     -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
// };

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

GLfloat vertices2[192];

std::vector<glm::vec3> cubePositions =
{
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

enum BlockType
{
    GRASS,
    WATER,
    SAND
};

struct BlockData
{
    glm::vec3 Position;
    BlockType Type;
};

int main()
{
    const int width = 1000;
    const int height = 1000;

    std::vector<BlockData> WorldData;
    
    int VerticesAmount;
    GLfloat* VerticesRef = Vertices::GetVertices(VerticesAmount);
    
    for (int i = 0; i < VerticesAmount; i++)
    {
        vertices2[i] = *(VerticesRef + i);
    }
    
    // Constants for the grid size and cube size
    // const int SIZE = 100;
    const float CUBE_SIZE = 1;
    const float THRESHOLD = 0.4;

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


    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            if (noiseValues[y][x] > THRESHOLD)
            {
                std::string numStr = std::to_string(noiseValues[y][x]);
                char afterDot = numStr[numStr.find('.') + 1];
                int value = afterDot - '0';

                BlockData blockData{};
                blockData.Position = glm::vec3(x * CUBE_SIZE, value * CUBE_SIZE, y * CUBE_SIZE);
                blockData.Type = GRASS;
                WorldData.insert(WorldData.end(), blockData);
            }

            else if (noiseValues[y][x] > 0.37 && noiseValues[y][x] < THRESHOLD)
            {
                std::string numStr = std::to_string(noiseValues[y][x]);
                char afterDot = numStr[numStr.find('.') + 1];
                int value = afterDot - '0';

                BlockData blockData{};
                blockData.Position = glm::vec3(x * CUBE_SIZE, value * CUBE_SIZE, y * CUBE_SIZE);
                blockData.Type = SAND;
                WorldData.insert(WorldData.end(), blockData);
            }

            else
            {
                std::string numStr = std::to_string(noiseValues[y][x]);
                char afterDot = numStr[numStr.find('.') + 1];
                int value = afterDot - '0';

                BlockData blockData{};
                blockData.Position = glm::vec3(x * CUBE_SIZE, value * CUBE_SIZE, y * CUBE_SIZE);
                blockData.Type = WATER;
                WorldData.insert(WorldData.end(), blockData);
            }
        }
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Window", nullptr, nullptr);
    if (!window)
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

    VBO vbo1(vertices2, sizeof vertices2);
    EBO ebo1(indices, sizeof indices);

    vao1.LinkAttrib(vbo1, 0, 3, GL_FLOAT, 8 * sizeof(float), nullptr);
    vao1.LinkAttrib(vbo1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    vao1.LinkAttrib(vbo1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    vao1.Unbind();
    vbo1.Unbind();
    ebo1.Unbind();

    // grass_top.TexUnit(shader_program, "topTexture", 0);
    // grass_top.Bind();

    Texture grass_top("block-top.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    Texture grass_bottom("water.jpg", GL_TEXTURE_2D, GL_TEXTURE2, GL_RGB, GL_UNSIGNED_BYTE);
    Texture grass_side("block.jpg", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGB, GL_UNSIGNED_BYTE);
    // grass_side.TexUnit(shader_program, "sideTexture", 1);
    //
    // grass_bottom.TexUnit(shader_program, "bottomTexture", 2);

    // Texture grass_side("block.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    // grass_side.TexUnit(shader_program, "tex1", 0);

    // Texture water("water.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    // water.TexUnit(shader_program, "tex0", 0);
    //
    // Texture sand("sand2.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    // sand.TexUnit(shader_program, "tex0", 0);

    glEnable(GL_DEPTH_TEST);

    Camera camera(width, height, glm::vec3(10.0f, 100.0f, 10.0f));

    shader_program.Activate();

    double lastTime = glfwGetTime();
    float deltaTime;

    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time
        double currentTime = glfwGetTime();
        deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;

        glClearColor(0.29f, 0.66f, 0.87f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.speed = 0.6f;
        camera.Inputs(window);
        float gravity = 9.8f;

        vao1.Bind();

        for (auto block : WorldData)
        {
            if (block.Type == GRASS)
            {
                // grass_top.Bind();
                grass_top.TexUnit(shader_program, "topTexture", 0);
                grass_bottom.TexUnit(shader_program, "bottomTexture", 1);
                grass_side.TexUnit(shader_program, "sideTexture", 2);

                glActiveTexture(GL_TEXTURE0);
                grass_top.Bind();
                
                glActiveTexture(GL_TEXTURE1);
                grass_bottom.Bind();

                glActiveTexture(GL_TEXTURE2);
                grass_side.Bind();
            }
            else if (block.Type == SAND)
            {
                // sand.Bind();
            }
            else if (block.Type == WATER)
            {
                // water.Bind();
            }

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(glm::mat4(1.0f),
                                   block.Position);

            camera.Matrix(80.0f, 0.1f, 300.0f, shader_program, "camMatrix", model);
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, nullptr);
        }

        glm::vec3 playerPos = glm::vec3(std::round(camera.GetPosX()), std::round(camera.GetPosY()),
                                        std::round(camera.GetPosZ()));
        glm::vec3 lowerBlock = glm::vec3(playerPos.x, playerPos.y - 2, playerPos.z);

        // std::cout << playerPos.x << ", " << playerPos.y << ", " << playerPos.z << std::endl;
        // std::cout << lowerBlock.x << ", " << lowerBlock.y << ", " << lowerBlock.z << std::endl << std::endl;

        bool bIsPlayerOnBlock = false;

        for (auto block : WorldData)
        {
            if (block.Position == lowerBlock)
            {
                bIsPlayerOnBlock = true;
                break;
            }
        }

        // if (!bIsPlayerOnBlock)
        // {
        //     camera.SetPosY(camera.GetPosY() - (gravity * deltaTime));
        // }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    vao1.Delete();
    vbo1.Delete();
    ebo1.Delete();

    grass_top.Delete();
    // water.Delete();

    shader_program.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
