#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "Shader.h"

class Texture
{
public:
    GLuint id;
    GLenum type;
    Texture(const char *image, GLenum tex_type, GLenum slot, GLenum format, GLenum pixel_type);
    
    // Texture(const char* topImage, const char* sideImage, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

    void TexUnit(Shader &shader, const char *uniform, GLuint unit);
    // void TexUnit(Shader& shader, const char* topUniform, const char* sideUniform, GLuint topUnit, GLuint sideUnit);
    void Bind();
    void Unbind();
    void Delete();
};

#endif