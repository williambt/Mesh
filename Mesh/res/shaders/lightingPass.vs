#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 2) in vec2 vTexCoord;

out vec2 TexCoords;

void main()
{
	gl_Position = vec4(vPos, 1.0);
	TexCoords = vTexCoord;
}
