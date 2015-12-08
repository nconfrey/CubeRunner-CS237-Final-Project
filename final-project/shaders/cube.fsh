#version 410 core

in vec3 f_normal;
in float distToCam;
out vec4 colorOut;

uniform vec3 lightDir;
uniform vec3 lightInten;
uniform vec3 lightAmb;

uniform int hasFog;
uniform vec3 fogColor;
uniform float fogDensity;

uniform vec4 color;

void main()
{   
    vec3 norm = normalize(f_normal); //renormalize the normal  


    colorOut = vec4((lightAmb + 0.5) + max(0, dot(-lightDir, norm)) * lightInten,1) * color;

    if(hasFog != 0){
		float ffog = exp2(-1.442695 * fogDensity * fogDensity * distToCam * distToCam);
		vec3 fog = (1.0 - ffog) * fogColor;
		colorOut = ffog * colorOut + vec4(fog, 1.0);
	}
}