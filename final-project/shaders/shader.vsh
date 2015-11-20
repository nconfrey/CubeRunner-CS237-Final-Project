/*! \file shader.vert
 *
 * \brief The vertex shader for a cube
 *
 * \author Lamont Samuels
 *
 */
#version 410 core

layout(location = 0) in vec4 position; 
layout(location = 1) in vec2 tCoord;
layout(location = 2) in vec2 nCoord;

uniform mat4 modelView; 
uniform mat4 projection;
uniform float hscale;
uniform float vscale;

out vec2 texCoord;
out vec2 normCoord; 


void main (void)
{
	vec3 pos = vec3(position.x * hscale, position.y * vscale, position.z * hscale);
    gl_Position =  projection * modelView * vec4(pos, 1.0);  

    texCoord = tCoord;
    normCoord = nCoord;
}
