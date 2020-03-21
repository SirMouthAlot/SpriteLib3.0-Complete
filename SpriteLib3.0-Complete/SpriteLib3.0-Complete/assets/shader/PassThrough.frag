#version 420

out vec4 FragColor;

in vec2 TexCoords;

layout (binding = 0) uniform sampler2D uTex;
uniform float uTransparency=1.0f;

void main() 
{
	vec4 source = texture(uTex, TexCoords);

	FragColor.rgb = source.rgb;
	FragColor.a = source.a * uTransparency;
}