#version 420

layout(location = 0) out vec4 FragColor;

in vec2 TexCoords;

layout (binding = 0) uniform sampler2D uTex;

void main() 
{
	vec4 source = texture(uTex, TexCoords);

	FragColor.rgb = source.rgb;
	FragColor.a = source.a;
}