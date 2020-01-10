#version 420

out vec4 FragColor;

in vec2 TexCoords;

layout(binding = 0) uniform sampler2D uTex;

//If the color on the screen is between these two numbers
//We keep it on screen
//if not we make it greyscale
uniform vec3 uColorRange1 = vec3(1.f);
uniform vec3 uColorRange2 = vec3(1.f);

bool CheckBetween(vec3 leftMost, vec3 isBetween, vec3 rightMost)
{
    if (isBetween.r >= leftMost.r && isBetween.g >= leftMost.g && isBetween.b >= leftMost.b)
    {
        if (isBetween.r <= rightMost.r && isBetween.g <= rightMost.g && isBetween.b <= rightMost.b)
            return true;
        else
            return false;
    }
    else
    {
        return false;
    }
}

void main() 
{
	vec4 source = texture(uTex, TexCoords);
	float luminence = 0.2989 * source.r + 0.587 * source.g + 0.114 * source.b;
	
    if (CheckBetween(uColorRange1, source.rgb, uColorRange2))
    {
        FragColor.rgb = source.rgb;
    }
    else
    {
	    FragColor.rgb = vec3(luminence);
    }
	FragColor.a = 1.0f;
}