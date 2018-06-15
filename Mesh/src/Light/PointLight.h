#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
	float constantAttenuation, linearAttenuation, quadraticAttenuation;

	PointLight(void)
		: constantAttenuation(0), linearAttenuation(1), quadraticAttenuation(0) {}
	PointLight(glm::vec3 position, glm::vec4 diffuse = glm::vec4(1), glm::vec4 specular = glm::vec4(1)) 
		: Light(position, diffuse, specular), constantAttenuation(0), linearAttenuation(1), quadraticAttenuation(0) {}

	virtual ~PointLight(void) {}

	virtual void ApplyToShader(const Shader& shader, int index)
	{
		std::string str = "lights[" + std::to_string(index) + "].";
		shader.Uniform4f((str + "position").c_str(), glm::vec4(position, 1.0f));
		shader.Uniform4f((str + "diffuse").c_str(),  diffuse);
		shader.Uniform4f((str + "specular").c_str(), specular);
		shader.Uniform1f((str + "constantAttenuation").c_str(), constantAttenuation);
		shader.Uniform1f((str + "linearAttenuation").c_str(), linearAttenuation);
		shader.Uniform1f((str + "quadraticAttenuation").c_str(), quadraticAttenuation);
	}
};