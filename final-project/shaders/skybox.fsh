#version 410 core

in vec3 TexCoords;
out vec4 color;

uniform sampler2D skybox;
uniform int side;

uniform int hasFog;
uniform vec3 fogColor;
uniform float fogDensity;

void main()
{   
	vec2 texSide; 
	switch(side)
	{
		//left and right
		case 0:
			texSide = vec2(1 - TexCoords.z, TexCoords.y);
			break;
		case 1:
			texSide = TexCoords.zy;
			break;
		//top and bottom
		case 2:
		case 3:
			texSide = TexCoords.xz;
			break;
		//front and back
		case 4:
			texSide = TexCoords.xy;
			break;
		case 5:
			texSide = vec2(1 - TexCoords.x, TexCoords.y);
	}
    color = texture(skybox, texSide);

    if(hasFog != 0){
		float ffog = exp2(-1.442695 * fogDensity * fogDensity * (1.0-gl_FragCoord.y) * (1.0-gl_FragCoord.y));
		vec3 fog = (1.0 - ffog) * fogColor;
		color = ffog * color + vec4(fog, 1.0);
	}
}