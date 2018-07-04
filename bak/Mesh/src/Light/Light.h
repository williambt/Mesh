#pragma once

#include <glm/glm.hpp>
#include "../Shader/Shader.h"

class Light
{
private:

public:
	glm::vec3 position;
	glm::vec4 diffuse;
	glm::vec4 specular;

	Light(void) : position(0), diffuse(1), specular(1) {}
	Light(glm::vec3 position, glm::vec4 diffuse = glm::vec4(1), glm::vec4 specular = glm::vec4(1)) : position(position), diffuse(diffuse), specular(specular) {}
	virtual ~Light(void) {}

	virtual void ApplyToShader(const Shader& shader, int index)
	{
		std::string str = "lights[" + std::to_string(index) + "].";
		shader.Uniform4f((str + "position").c_str(), glm::vec4(position, 0.0f));
		shader.Uniform4f((str + "diffuse").c_str() , diffuse);
		shader.Uniform4f((str + "specular").c_str(), specular);
	}
};