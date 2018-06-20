#pragma once

#include <glm/mat4x4.hpp>

class Shader
{
private:
	unsigned int _program;

public:
	Shader() {}
	Shader(const char* vsPath, const char* fsPath);
	~Shader(void);

	void Load(const char* vsPath, const char* fsPath);

	void Bind() const;
	void Unbind() const;

	int GetUniformLocation(const char* name) const;

	void Uniform1i(const char* name, const int val) const;
	void Uniform1ui(const char* name, const unsigned int val) const;
	void Uniform1f(const char* name, const float val) const;
	void Uniform2f(const char* name, const glm::vec2& vec) const;
	void Uniform3f(const char* name, const glm::vec3& vec) const;
	void Uniform4f(const char* name, const glm::vec4& vec) const;
	void UniformMat3f(const char* name, const glm::mat3& mat) const;
	void UniformMat4f(const char* name, const glm::mat4& mat) const;


	inline unsigned int GetProgram() const { return _program; }
};