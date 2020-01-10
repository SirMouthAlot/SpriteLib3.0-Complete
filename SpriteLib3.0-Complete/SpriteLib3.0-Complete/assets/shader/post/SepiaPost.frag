#version 420

out vec4 FragColor;

in vec2 TexCoords;

layout(binding = 0) uniform sampler2D uTex;

//Intensity of the Sepia
uniform float uIntensity = 0.8;

void main() 
{
	vec4 source = texture(uTex, TexCoords);
	
	vec3 sepiaColor;
	sepiaColor.r = ((source.r * 0.393) + (source.g * 0.769) + (source.b * 0.189));
	sepiaColor.g = ((source.r * 0.349) + (source.g * 0.686) + (source.b * 0.168));
	sepiaColor.b = ((source.r * 0.272) + (source.g * 0.534) + (source.b * 0.131));
	
	FragColor.rgb = mix(source.rgb, sepiaColor.rgb, uIntensity);
	FragColor.a = 1.0f;
}