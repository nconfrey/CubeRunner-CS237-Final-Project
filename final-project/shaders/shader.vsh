/*! \file shader.vert
 *
 * \brief The vertex shader for a cube
 *
 * \author Lamont Samuels
 *
 */
#version 410 core

layout(location = 0) in vec4 position; 
//layout(location = 1) in vec2 tCoord;
//layout(location = 2) in vec2 nCoord;

uniform mat4 modelView; 
uniform mat4 projection;
uniform float hscale;
uniform float vscale;
uniform vec3 nw_pos;
uniform vec3 nwtile;
uniform float cellwidth;

out vec2 texCoord;
out vec2 normCoord; 


void main (void)
{
	vec3 pos = vec3((position.x + nw_pos.x) * hscale, position.y * vscale, (position.z + nw_pos.z) * hscale);
	//pos = vec3(position.x * hscale, position.y * vscale, position.z * hscale);
    gl_Position =  projection * modelView * vec4(pos, 1.0);  


    //calculate tex coords
    texCoord = vec2((position.x - nwtile.x)/ cellwidth, (position.z - nwtile.z)/ cellwidth);
    normCoord = vec2((position.x - nwtile.x)/ cellwidth, (position.z - nwtile.z) / cellwidth);
}
