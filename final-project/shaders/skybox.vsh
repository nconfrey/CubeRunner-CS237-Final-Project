#version 410 core
layout (location = 0) in vec3 position;
out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 modelView;
uniform vec3 camPos;

uniform float ht;
uniform float wid;

void main()
{
	vec3 stretchedpos = vec3((position.x * wid) + camPos.x, (position.y * ht) + camPos.y, (position.z * wid) + camPos.z);
	
    gl_Position = projection * modelView * vec4(stretchedpos, 1);
    TexCoords = (position + vec3(1.0,1.0,1.0)) / 2.0;
} 