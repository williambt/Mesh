#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gColour;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D uTexture;

void main()
{
	gNormal = normalize(Normal);
	gColour = texture(uTexture, TexCoord).rgb;
	gPosition = FragPos;
}