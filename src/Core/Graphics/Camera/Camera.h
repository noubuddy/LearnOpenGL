﻿#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "../Shader/Shader.h"

class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

    bool firstClick = true;
    
    int width;
    int height;

    float speed = 0.6f;
    float sensitivity = 100.0f;

    Camera(int width, int height, glm::vec3 position);

    glm::mat4 GetViexMatrix();
    void Matrix(float fov_deg, float near_plane, float far_plane, Shader &shader, const char *uniform, glm::mat4 model);
    void Inputs(GLFWwindow *window);

    // getters
    float GetPosX();
    float GetPosY();
    float GetPosZ();

    // setters
    void SetPosX(float newPosX);
    void SetPosY(float newPosY);
    void SetPosZ(float newPosZ);
};


#endif