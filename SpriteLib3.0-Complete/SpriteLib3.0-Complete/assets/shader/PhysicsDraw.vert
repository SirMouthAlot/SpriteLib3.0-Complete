#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoords;

uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uModel;

void main()
{
	gl_Position = uProj * uView * uModel * vec4(position, 1.0);
	TexCoords.x = texCoord.x;
    TexCoords.y = 1.0 - texCoord.y;   
}