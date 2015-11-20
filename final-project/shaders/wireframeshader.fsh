/*! \file shader.vert
 *
 * \brief The fragment shader for a cube 
 *
 * \author Lamont Samuels 
 *
 */
#version 410 core 

uniform vec4 color;
out vec4 fragcolor;

void main(void)
{
	fragcolor = color;
}