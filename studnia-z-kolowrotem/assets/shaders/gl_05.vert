#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texCoord;

out vec3 vecColor;
out vec2 TexCoord;
  
uniform mat4 model;
uniform mat4 MVP;

void main()
{
    gl_Position = MVP * model * vec4(position, 1.0f);
    vecColor = position;
    TexCoord = texCoord;
}