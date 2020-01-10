#version 420

out vec4 FragColor;

uniform vec4 uColor;

in vec2 TexCoords;

layout (binding = 0) uniform sampler2D uTex;

//Set the color
void main() 
{
	vec4 mask = texture(uTex, TexCoords);

	FragColor.rgb = uColor.rgb * mask.rgb;
	FragColor.a = uColor.a * mask.a;
}