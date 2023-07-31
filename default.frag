#version 330 core

out vec4 FragColor;
in vec3 color;
in vec2 texCoord;

uniform sampler2D tex0;
uniform sampler2D tex1;

uniform sampler2D topTexture;
uniform sampler2D bottomTexture;
uniform sampler2D sideTexture;

void main()
{
    vec4 topColor = texture(topTexture, texCoord);
    vec4 bottomColor = texture(bottomTexture, texCoord);
    vec4 sideColor = texture(sideTexture, texCoord);

    if (gl_FrontFacing) 
    {
        if (gl_FragCoord.z > 0.0) 
        {
            FragColor = topColor;
        } 
        else 
        {
            FragColor = bottomColor;
        }
    } 
    else 
    {
        FragColor = sideColor;
    }
//    FragColor = texture(tex0, texCoord);
    
//    if (texCoord.y >= 0.95)  // Top face
//        FragColor = texture(tex1, texCoord);
//    else  // Side faces
//        FragColor = texture(tex1, texCoord);
}