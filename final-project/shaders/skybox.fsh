#version 410 core

in vec3 TexCoords;
out vec4 color;

uniform sampler2D skybox;

void main()
{    
    color = texture(skybox, TexCoords.xy);
}