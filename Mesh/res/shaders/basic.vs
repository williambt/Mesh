#version 330 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec3 vn;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

uniform mat4 pMat;
uniform mat4 vMat;
uniform mat4 mMat;

out VS_OUT {
	vec3 normal;
	vec2 uv;
	vec4 worldPos;
	mat3 TBN;
} vs_out;

out mat4 viewMatrix;

void main()
{
	gl_Position = pMat * vMat * mMat * pos;
	
	vs_out.uv = texCoord;
	vs_out.normal = vn;
	
	vs_out.worldPos = mMat * pos;
	viewMatrix = vMat;
	
	vec3 T = normalize(vec3(mMat * vec4(tangent, 0.0)));
	vec3 N = normalize(vec3(mMat * vec4(vn, 0.0)));
	vec3 B = cross(N, T);
	vs_out.TBN = mat3(T, B, N);
}