#version 330 core

layout (location = 0) in vec4 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;
layout (location = 3) in vec3 Tangent;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
out mat3 TBN;

uniform mat4 mMat;
uniform mat4 pMat;
uniform mat4 vMat;

void main()
{
	vec4 worldPos = mMat * vPos;
	gl_Position = pMat * vMat * worldPos;
	FragPos = worldPos.xyz;
	TexCoord = vTexCoord;
	Normal = transpose(inverse(mat3(mMat))) * vNormal;
	
	vec3 T = normalize(vec3(mMat * vec4(Tangent, 0.0)));
	vec3 N = normalize(vec3(mMat * vec4(vNormal, 0.0)));
	vec3 B = cross(N, T);
	TBN = mat3(T, B, N);
}
