#version 420

layout(binding = 0) uniform sampler2D uTex; //Source image
uniform float uThreshold;

out vec4 FragColor;

in vec2 TexCoords;

void main() 
{
	vec4 color = texture(uTex, TexCoords);
	
	float luminance = (color.r + color.g + color.b) / 3.0;
	
	if (luminance > uThreshold) 
	{
		FragColor = color;
	}
	else
	{
		FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}