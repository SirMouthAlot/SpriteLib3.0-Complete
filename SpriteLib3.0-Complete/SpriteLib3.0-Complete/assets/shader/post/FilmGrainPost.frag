#version 420

out vec4 FragColor;

in vec2 TexCoords;

layout(binding = 0) uniform sampler2D uTex;

uniform float uStrength = 32.0;
uniform float uTime;
uniform vec2 uWindowSize;   

void main() 
{
    //Create new more pixelated UV
    vec2 uv = gl_FragCoord.xy / uWindowSize;
    //Load in the source image with new uvs
	vec4 source = texture(uTex, uv);

    //Scrolls the uvs used to create the grain
    float x = (uv.x + 4.0 ) * (uv.y + 4.0 ) * (uTime * 10.0);
    //Creates the grain
	vec4 grain = vec4(mod((mod(x, 13.0) + 1.0) * (mod(x, 123.0) + 1.0), 0.01)-0.005) * uStrength;

    //apply grain to the image
    grain = 1.0 - grain;
	FragColor.rgb = source.rgb * grain.rgb;
	FragColor.a = 1.0f;
}