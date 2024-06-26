#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Indices.h"
#include "PerlinNoise.h"
#include "Renderer.h"
#include "TextureArray.h"
#include "Vertices.h"

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
    const int width = 1920;
    const int height = 1080;

    std::vector<BlockData> WorldData;

    // std::vector<GLfloat> VerticesRef = Vertices::GetVertices();

    std::vector<GLfloat> VerticesFront = Vertices::GetVerticesBySide(front_face);
    std::vector<GLfloat> VerticesBack = Vertices::GetVerticesBySide(back_face);
    std::vector<GLfloat> VerticesTop = Vertices::GetVerticesBySide(top_face);
    std::vector<GLfloat> VerticesBottom = Vertices::GetVerticesBySide(bottom_face);
    std::vector<GLfloat> VerticesRight = Vertices::GetVerticesBySide(right_face);
    std::vector<GLfloat> VerticesLeft = Vertices::GetVerticesBySide(left_face);

    std::vector<GLuint> Indices = Indices::GetIndicesOfSide();

    // Constants for the grid size and cube size
    const float CUBE_SIZE = 1.f;
    const float THRESHOLD = 0.4f;

    // Constants for the noise parameters
    const float NOISE_SCALE = 0.05f;

    // Initialize the Perlin noise object
    PerlinNoise noise;
    std::vector<std::vector<float>> noiseValues(GRID_SIZE, std::vector<float>(GRID_SIZE));

    // Fill noiseValues array
    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            for (int z = 0; z < GRID_SIZE; ++z)
            {
                noiseValues[y][x] = noise.noise(x * NOISE_SCALE, y * NOISE_SCALE, z * NOISE_SCALE);
            }
        }
    }

    // Generate world array
    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            float noiseValue = noiseValues[y][x];

            // get first digit after the dot
            float afterDotValue = static_cast<int>(std::floor(std::fabs(noiseValue) * 10)) % 10;

            BlockData blockData{};
            blockData.Position = glm::vec3(x * CUBE_SIZE, afterDotValue * CUBE_SIZE, y * CUBE_SIZE);

            if (noiseValue > THRESHOLD)
            {
                blockData.Type = GRASS;
            }
            else if (noiseValue > 0.37 && noiseValue < THRESHOLD)
            {
                blockData.Type = SAND;
            }
            else
            {
                blockData.Type = WATER;
            }

            WorldData.push_back(blockData);
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

    // Texture grass_top_texture("block-top.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    // grass_top_texture.TexUnit(shader_program, "tex0", 0);
    //
    // Texture grass_side_texture("block.jpg", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGB, GL_UNSIGNED_BYTE);
    // grass_side_texture.TexUnit(shader_program, "tex1", 1);
    //
    // Texture water("water.jpg", GL_TEXTURE_2D, GL_TEXTURE2, GL_RGB, GL_UNSIGNED_BYTE);
    // water.TexUnit(shader_program, "tex0", 2);
    //
    // Texture sand("sand2.jpg", GL_TEXTURE_2D, GL_TEXTURE3, GL_RGB, GL_UNSIGNED_BYTE);
    // sand.TexUnit(shader_program, "tex0", 3);

    std::vector<std::string> image_paths =
    {
        "block.jpg", "block.jpg", "block.jpg",
        "block.jpg", "block-top.jpg", "block.jpg"
    };
    TextureArray texture_array(image_paths);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array.id);

    glUseProgram(shader_program.id);
    glUniform1i(glGetUniformLocation(shader_program.id, "texArray"), 0);

    Renderer renderer;

    Texture grass_top_texture("block-top.jpg", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGB, GL_UNSIGNED_BYTE);

    std::vector<GLfloat> v = Vertices::GetVertices();
    std::vector<GLuint> i = Indices::GetIndices();

    renderer.PushRenderData(v, i, grass_top_texture);

    // renderer.PushRenderData(VerticesFront, Indices, grass_side_texture);
    // renderer.PushRenderData(VerticesBack, Indices, grass_side_texture);
    // renderer.PushRenderData(VerticesTop, Indices, grass_top_texture);
    // renderer.PushRenderData(VerticesBottom, Indices, grass_top_texture);
    // renderer.PushRenderData(VerticesRight, Indices, grass_side_texture);
    // renderer.PushRenderData(VerticesLeft, Indices, grass_side_texture);

    Camera camera(width, height, glm::vec3(10.0f, 100.0f, 10.0f));

    shader_program.Activate();

    glEnable(GL_DEPTH_TEST);

    double lastTime = glfwGetTime();
    int nbFrames = 0;

    while (!glfwWindowShouldClose(window))
    {
        // Measure speed
        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0)
        {
            // If last prinf() was more than 1 sec ago
            // printf and reset timer
            // printf("%f ms/frame, %i FPS\n", 1000.0 / double(nbFrames), nbFrames);
            std::cout << 1000.0 / double(nbFrames) << " ms/frame, " << nbFrames << " FPS\n";
            nbFrames = 0;
            lastTime += 1.0;
        }

        glClearColor(0.29f, 0.66f, 0.87f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.speed = 0.6f;
        camera.Inputs(window);

        for (auto block : WorldData)
        {
            // switch (block.Type)
            // {
            //     case GRASS:
            //         grass_top.Bind();
            //         break;
            //     case SAND:
            //         sand.Bind();
            //         break;
            //     case WATER:
            //         water.Bind();
            //         break;
            // }

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(glm::mat4(1.0f), block.Position);

            camera.Matrix(80.0f, 0.1f, 300.0f, shader_program, "camMatrix", model);

            renderer.Render();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // vao1.Delete();
    // vbo1.Delete();
    // ebo1.Delete();

    // grass_top.Delete();
    // water.Delete();

    shader_program.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
