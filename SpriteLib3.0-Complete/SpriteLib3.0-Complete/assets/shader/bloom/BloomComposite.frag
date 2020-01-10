#version 420

layout(binding = 0) uniform sampler2D uScene;
layout(binding = 1) uniform sampler2D uBloom;

in vec2 TexCoords;

out vec4 FragColor;

void main() 
{
	vec4 colorA = texture(uScene, TexCoords);
	vec4 colorB = texture(uBloom, TexCoords);

	FragColor = 1.0 - (1.0 - colorA) * (1.0 - colorB);
	//FragColor = colorB;
}