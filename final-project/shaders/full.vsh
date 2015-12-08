#version 410

vec2 texs[6] = vec2[]( vec2(0,0),
						vec2(1,1),
						vec2(0,1),
						vec2(0,0),
						vec2(1,0),
						vec2(1,1));
/*
vec3 verts[6] = vec3[6](vec3(-.5,-.5,0),
						vec3(.5,.5,0),
						vec3(-.5,.5,0),
						vec3(-.5,-.5,0),
						vec3(.5,-.5,0),
						vec3(.5,.5,0));
*/

vec3 verts[6] = vec3[6](vec3(-1,-1,0),
						vec3(-.75,-.90,0),
						vec3(-1,-.90,0),
						vec3(-1,-1,0),
						vec3(-.75,-1,0),
						vec3(-.75,-.90,0));

out vec2 texCoord;

void main ()
{
	texCoord = texs[gl_VertexID];
	gl_Position = vec4(verts[gl_VertexID], 1);
}