#pragma once

#include <string>
#include <glm/vec3.hpp>
#include "../Shader/Shader.h"
#include "../Texture/Texture.h"

class Material
{
public:
	std::string name;

	glm::vec3 ambient, diffuse, specular;
	float shine;

	bool hasNormalMap;
	Texture diffuseTexture, normalMap;

	Material(void) {}
	Material(const std::string& name) { this->name = name; }

	inline void ApplyToShader(const Shader& shader)
	{
		shader.Uniform3f("mtl.ambient", ambient);
		shader.Uniform3f("mtl.diffuse", diffuse);
		shader.Uniform3f("mtl.specular", specular);
		shader.Uniform1f("mtl.shine", shine);
		shader.Uniform1i("uTexture", 0);
		shader.Uniform1i("hasNormalMap", hasNormalMap);
		if (hasNormalMap)
			shader.Uniform1i("uNormalMap", 1);
	}
};