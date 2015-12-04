#version 410 core

in vec3 TexCoords;
out vec4 color;

uniform samplerCube skybox;

void main()
{    
    color = texture(skybox, TexCoords);
    //color = vec4(.5,.75,.9,1);
}