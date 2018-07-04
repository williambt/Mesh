#version 330 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec3 vn;
layout (location = 2) in vec2 texCoord;

uniform mat4 pMat;
uniform mat4 mMat;

out vec3 normal;
out vec2 uv;
out vec4 worldPos;

void main()
{
	gl_Position = pMat * mMat * pos;
	normal = vn;
	uv = texCoord;
	
	worldPos = mMat * pos;
}