#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <string>
#include <fstream>
#include <ios>
#include <glad/glad.h>
#include <iostream>

class Shader
{
    
public:
    unsigned int id;
    
    Shader(const char* vertex_shader_file, const char* fragment_shader_file);

    void Activate();

    void Delete();

private:
    std::string ReadFile(const char* filename);

    bool CompileShader(unsigned int shader);
};

#endif