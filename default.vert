#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in float aTexIndex;

out vec3 color;
out vec2 texCoord;
flat out float texIndex;

uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
    gl_Position = camMatrix * vec4(aPos, 1.0f);
    
    color = aColor;
    texCoord = aTex;
    texIndex = aTexIndex;
}