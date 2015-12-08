#version 410

in vec2 texCoord;

uniform sampler2D image;
uniform vec2 screenSize;

out vec4 fragColor;

void main()
{
	vec2 texCoords = gl_FragCoord.xy / screenSize;
	fragColor = texture(image, texCoord);
	fragColor = vec4(fragColor.xyz, 0);
	//fragColor = texture(image, texCoords);
	//fragColor = vec4(.75, .2, .5, 1);
}