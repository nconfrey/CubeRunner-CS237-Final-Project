/*! \file shader.vert
 *
 * \brief The vertex shader for a cube
 *
 * \author Lamont Samuels
 *
 */
#version 410 core

layout(location = 0) in vec4 position; 

uniform mat4 modelView; 
uniform mat4 projection;
uniform float hscale;
uniform float vscale;


void main (void)
{
	vec3 pos = vec3(position.x * hscale, position.y * vscale, position.z * hscale);
    gl_Position =  projection * modelView * vec4(pos, 1.0);  
}
