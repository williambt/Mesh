#version 330 core

layout (location = 0) out vec4 frag_colour;

uniform vec4 colour;

void main()
{
	frag_colour = colour;
}