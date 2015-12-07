#version 410 core

in vec3 f_normal;
out vec4 colorOut;

uniform vec3 lightDir;
uniform vec3 lightInten;
uniform vec3 lightAmb;

uniform vec4 color;

void main()
{   
    vec3 norm = normalize(f_normal); //renormalize the normal  

    /* This is the color for the fragment without lighting. You will use the lighting equation 
      defined in the project to compute the frag color with lighting included. */ 

    colorOut = vec4((lightAmb + 0.5) + max(0, dot(-lightDir, norm)) * lightInten,1) * color;
}