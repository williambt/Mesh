#pragma once
#include "PointLight.h"
#include <string>

class SpotLight : public PointLight
{
public:
	float spotCutoff, spotExponent;
	glm::vec3 spotDirection;

	SpotLight(void) : PointLight() {}
	SpotLight(glm::vec3 position = glm::vec3(0), glm::vec3 direction = glm::vec3(0, 0, -1)) : PointLight(position), spotDirection(direction) 
		{}

	virtual void ApplyToShader(const Shader& shader, int index)
	{
		std::string str = "lights[" + std::to_string(index) + "].";
		shader.Uniform4f((str + "position").c_str(), glm::vec4(position, 1.0f));
		shader.Uniform4f((str + "diffuse").c_str(), diffuse);
		shader.Uniform4f((str + "specular").c_str(), specular);
		shader.Uniform1f((str + "constantAttenuation").c_str(), constantAttenuation);
		shader.Uniform1f((str + "linearAttenuation").c_str(), linearAttenuation);
		shader.Uniform1f((str + "quadraticAttenuation").c_str(), quadraticAttenuation);
		shader.Uniform1f((str + "spotCutoff").c_str(), spotCutoff);
		shader.Uniform1f((str + "spotExponent").c_str(), spotExponent);
		shader.Uniform3f((str + "spotDirection").c_str(), spotDirection);
	}
};