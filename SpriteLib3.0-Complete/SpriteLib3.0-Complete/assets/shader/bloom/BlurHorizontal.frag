#version 420

layout(binding = 0) uniform sampler2D uTex; //Source image
uniform float uPixelSize; //1.0 / Window_Width

out vec4 FragColor;

in vec2 TexCoords;

void main() 
{
	//Sample pixels in a horizontal row
	//Weight should add up to 1
	FragColor = vec4(0.0, 0.0, 0.0, 0.0);

	FragColor += texture(uTex, vec2(TexCoords.x - 4.0 * uPixelSize, TexCoords.y)) * 0.06;
	FragColor += texture(uTex, vec2(TexCoords.x - 3.0 * uPixelSize, TexCoords.y)) * 0.09;
	FragColor += texture(uTex, vec2(TexCoords.x - 2.0 * uPixelSize, TexCoords.y)) * 0.12;
	FragColor += texture(uTex, vec2(TexCoords.x - 		uPixelSize, TexCoords.y)) * 0.15;
	FragColor += texture(uTex, vec2(TexCoords.x, 					TexCoords.y)) * 0.16;
	FragColor += texture(uTex, vec2(TexCoords.x + 		uPixelSize, TexCoords.y)) * 0.15;
	FragColor += texture(uTex, vec2(TexCoords.x + 2.0 * uPixelSize, TexCoords.y)) * 0.12;
	FragColor += texture(uTex, vec2(TexCoords.x + 3.0 * uPixelSize, TexCoords.y)) * 0.09;
	FragColor += texture(uTex, vec2(TexCoords.x + 4.0 * uPixelSize, TexCoords.y)) * 0.06;
}