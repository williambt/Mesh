#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
private:
	glm::vec3 _position;
	glm::vec3 _center;

	glm::mat4 _matrix;

	bool _changed;

public:
	Camera(glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 center = glm::vec3(0, 0, -1)) : _position(position), _center(center), _changed(true) {};
	~Camera(void) {}

	inline glm::vec3 GetPosition() { return _position; }
	inline glm::vec3 GetCenter() { return _center; }

	inline void SetPosition(glm::vec3 pos) { _position = pos; _changed = true; }
	inline void SetCenter(glm::vec3 center) { _center = center; _changed = true; }

	inline void Translate(glm::vec3 vec) { _position += vec; _changed = true; }
	inline void TranslateCenter(glm::vec3 vec) { _center += vec; _changed = true; }

	inline void RotateCenterY(float angle)
	{
		angle = glm::radians(angle);
		_center -= _position;
		float c = cosf(angle), s = sinf(angle);
		float newX = c * _center.x - s * _center.z;
		float newZ = s * _center.x + c * _center.z;
		_center.x = newX;
		_center.z = newZ;
		_center += _position;
		_changed = true;
	}

	inline void RotateCenterX(float angle)
	{
		angle = glm::radians(angle);
		_center -= _position;
		float c = cosf(angle), s = sinf(angle);
		float newY = c * _center.y - s * _center.z;
		float newZ = s * _center.y + c * _center.z;
		_center.y = newY;
		_center.z = newZ;
		_center += _position;
		_changed = true;
	}

	inline glm::vec3 GetForward()
	{
		glm::vec3 forward(-_matrix[0][2], -_matrix[1][2], -_matrix[2][2]);
		return forward;
	}

	inline glm::vec3 GetRight()
	{
		glm::vec3 right(_matrix[0][0], _matrix[1][0], _matrix[2][0]);
		return right;
	}

	inline glm::mat4 GetMatrix() 
	{
		if (_changed)
			_matrix = glm::lookAt(_position, _center, glm::vec3(0, 1, 0));
		return _matrix;
	}
};

class Camera2
{
private:
	glm::vec3 _position;
	glm::vec3 _right;
	glm::vec3 _forwards;
	glm::vec3 _up;

	float xrot = 0, yrot = 0;

	glm::mat4 _matrix;

	bool _changed;

public:
	Camera2(glm::vec3 position = glm::vec3(0, 0, 0)) : _position(position), _right(1, 0, 0), _forwards(0, 0, -1), _up(0, 1, 0), _matrix(1), _changed(true) {};
	~Camera2(void) {}

	inline glm::vec3 GetPosition() { return _position; }

	inline void SetPosition(glm::vec3 pos) { _position = pos; _changed = true; }

	inline void Translate(glm::vec3 vec) { _position += vec; _changed = true; }

	inline void Rotate(glm::vec3 rotation)
	{
		rotation = glm::radians(rotation);
		xrot += rotation.x; yrot += rotation.y;
		_forwards.x = cosf(yrot) * cosf(xrot);
		_forwards.y = sinf(xrot);
		_forwards.z = sinf(yrot) * cosf(xrot);
		_forwards = glm::normalize(_forwards);

		_changed = true;
	}

	inline glm::vec3 GetUp()
	{
		return _up;
	}

	inline glm::vec3 GetForward()
	{
		return _forwards;
	}

	inline glm::vec3 GetRight()
	{
		return _right;
	}

	inline glm::mat4 GetMatrix()
	{
		if (_changed)
		{
			/*_matrix[0][0] = _right.x; _matrix[1][0] = _right.y; _matrix[2][0] = _right.z;
			_matrix[0][1] = _up.x; _matrix[1][1] = _up.y; _matrix[2][1] = _up.z;
			_matrix[0][2] = -_forwards.x; _matrix[1][2] = -_forwards.y; _matrix[2][2] = -_forwards.z;
			_matrix[3][0] = -_position.x; _matrix[3][1] = -_position.y; _matrix[3][2] = -_position.z;*/
			_matrix = glm::lookAt(_position, _position + _forwards, glm::vec3(0, 1, 0));
		}
			//_matrix = glm::lookAt(_position, _center, glm::vec3(0, 1, 0));
		return _matrix;
	}
};