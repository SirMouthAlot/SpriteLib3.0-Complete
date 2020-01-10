#version 420

out vec4 FragColor;

in vec2 TexCoords;

layout(binding = 0) uniform sampler2D uTex;

uniform float uPixelSize = 8.0;
uniform vec2 uWindowSize;

void main() 
{
    //Create new more pixelated UV
    vec2 uv = floor(gl_FragCoord.xy / uPixelSize + 0.5) * uPixelSize;
    //Load in the source image with new uvs
	vec4 source = texture(uTex, uv / uWindowSize);

    //Output the newly pixelated image
	FragColor.rgb = source.rgb;
	FragColor.a = 1.0f;
}