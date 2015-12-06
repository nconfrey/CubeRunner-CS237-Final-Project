#version 410 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 projection;
uniform mat4 modelView;

uniform float ht;
uniform float wid;

out vec3 f_normal;

void main()
{
	f_normal = normal;
	vec3 stretchedpos = vec3((position.x * wid) , (position.y * ht), (position.z * wid));
    gl_Position = projection * modelView * vec4(stretchedpos, 1);
}