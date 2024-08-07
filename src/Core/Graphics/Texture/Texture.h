﻿#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "../Shader/Shader.h"

class Texture
{
public:
    GLuint id;
    GLenum type;
    Texture(const char *image, GLenum tex_type, GLenum slot, GLenum format, GLenum pixel_type);

    void TexUnit(Shader &shader, const char *uniform, GLuint unit);
    void Bind();
    void Unbind();
    void Delete();
};

#endif