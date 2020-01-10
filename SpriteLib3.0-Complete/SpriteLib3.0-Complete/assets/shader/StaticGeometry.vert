#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec2 TexCoords;
out vec3 normal0;
out vec3 fragPosition;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

void main()
{
	gl_Position = uProj * uView * uModel * vec4(position, 1.0);
	TexCoords.x = texCoord.x;
    TexCoords.y = 1.0 - texCoord.y;   
	normal0 = mat3(uView) * mat3(uModel) * normal;
	fragPosition = (uView * uModel * vec4(position, 1.0)).rgb;
}