#version 410 core

in vec3 TexCoords;
out vec4 color;

uniform sampler2D skybox;
uniform int side;

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
}