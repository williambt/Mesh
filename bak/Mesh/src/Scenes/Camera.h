#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
private:
	glm::vec3 _position;
	glm::vec3 _center;

public:
	Camera(glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 center = glm::vec3(0, 0, -1)) : _position(position), _center(center) {};
	~Camera(void) {}

	inline glm::vec3 GetPosition() const { return _position; }
	inline glm::vec3 GetCenter() const { return _center; }

	inline void SetPosition(glm::vec3 pos) { _position = pos; }
	inline void SetCenter(glm::vec3 center) { _center = center; }

	inline void Translate(glm::vec3 vec) { _position += vec; }
	inline void TranslateCenter(glm::vec3 vec) { _center += vec; }

	inline glm::mat4 GetMatrix() const { return glm::lookAt(_position, _center, glm::vec3(0, 1, 0)); }
};