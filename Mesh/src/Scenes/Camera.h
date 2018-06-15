#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Transform.h"

class Camera
{
private:
	Transform _transform;

	glm::mat4 _matrix;

	glm::vec3 _forwardVec;

	bool _changed;

	inline void ApplyRotation()
	{
		float xrot = glm::radians(_transform.GetRotation().x);
		float yrot = glm::radians(_transform.GetRotation().y);
		_forwardVec.x = cosf(yrot) * cosf(xrot);
		_forwardVec.y = sinf(xrot);
		_forwardVec.z = sinf(yrot) * cosf(xrot);
		_forwardVec = glm::normalize(_forwardVec);
	}

public:
	Camera(glm::vec3 position = glm::vec3(0, 0, 0)) : _transform(position), _changed(true) {};
	~Camera(void) {}

	inline Transform& GetTransform() { return _transform; }

	inline glm::vec3 GetForward()
	{
		return _forwardVec;
	}

	inline glm::vec3 GetRight()
	{
		glm::vec3 right(_matrix[0][0], _matrix[1][0], _matrix[2][0]);
		return right;
	}

	inline glm::mat4 GetMatrix() 
	{
		if (_transform.HasChanged())
		{
			ApplyRotation();
			_matrix = glm::lookAt(_transform.GetPosition(), _transform.GetPosition() + _forwardVec, glm::vec3(0, 1, 0));
			_transform.RemoveChanged();
		}
		return _matrix;
	}
};