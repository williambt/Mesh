#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gColour;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in mat3 TBN;

uniform sampler2D uTexture;
uniform int hasNormalMap;
uniform sampler2D uNormalMap;

void main()
{
	vec3 normal;
	if(hasNormalMap == 1)
	{
		normal = texture(uNormalMap, TexCoord).rgb;
		normal = normalize(normal * 2.0 - 1.0);
		normal = normalize(TBN * normal);
	}
	else
		normal = Normal;
	gNormal = normalize(normal);
	gColour = texture(uTexture, TexCoord).rgb;
	gPosition = FragPos;
}