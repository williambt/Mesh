#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct Vertex
{
public:
	glm::vec3 vert;
	glm::vec2 texCoord;
	glm::vec3 normal;

	Vertex() : vert(0.0f), texCoord(0.0f), normal(0.0f) {}
	Vertex(glm::vec3 vert) : vert(vert), texCoord(0.0f), normal(0.0f) {}
	Vertex(glm::vec3 vert, glm::vec2 texCoord) : vert(vert), texCoord(texCoord), normal(0.0f) {}
	Vertex(glm::vec3 vert, glm::vec3 normal) : vert(vert), texCoord(0.0f), normal(normal) {}
	Vertex(glm::vec3 vert, glm::vec2 texCoord, glm::vec3 normal) : vert(vert), texCoord(texCoord), normal(normal) {}

	inline bool operator==(const Vertex& other) { return vert == other.vert && texCoord == other.texCoord && normal == other.normal; }
};