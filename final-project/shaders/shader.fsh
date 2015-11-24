/*! \file shader.vert
 *
 * \brief The fragment shader for a cube 
 *
 * \author Lamont Samuels 
 *
 */
#version 410 core 

uniform vec3 lightDir;
uniform vec3 lightInten;
uniform vec3 lightAmb;

uniform sampler2D texSampler;
uniform sampler2D normSampler;

in vec2 texCoord;
in vec2 normCoord; 

out vec4 fragColor;

void main(void)
{
	vec4 texcolor = texture(texSampler, texCoord);
	vec4 normal = texture(normSampler, normCoord);
	normal = normalize(vec4(normal.x, normal.z, normal.y, 1.0));
	normal = normal * 2; //convert from [0..1] to [-1..1]
	normal = normal - 1;

	vec4 lighting = vec4((lightAmb + max(0, dot((lightDir),normalize(vec3(normal)))) * lightInten), 1.0);

	fragColor = lighting * texcolor;
	//fragColor = lighting * vec4(0.0, 0.313, 0.85, 1.0); //debug
}