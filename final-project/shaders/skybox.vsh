#version 410 core
layout (location = 0) in vec3 position;
out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 modelView;

void main()
{
    gl_Position = projection * modelView * vec4(position, 1);
    TexCoords = position;
} 