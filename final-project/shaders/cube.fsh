#version 410 core

in vec3 TexCoords;
out vec4 colorOut;

uniform vec4 color;

void main()
{   
    colorOut = color;
}