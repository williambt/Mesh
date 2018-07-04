#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
private:
	glm::vec3 _position;
	glm::vec3 _rotation;
	glm::vec3 _scale;

	glm::mat4 _translationMatrix;
	glm::mat4 _rotationMatrix;
	glm::mat4 _scaleMatrix;
	glm::mat4 _modelMatrix;

	struct 
	{
		bool position, rotation, scale;

		operator bool() { return (position || rotation || scale); }

		void operator=(bool other) { position = rotation = scale = other; }

	} _changed;

	inline void Recalculate()
	{
		if (_changed.position)
			_translationMatrix = glm::translate(glm::mat4(1), _position);
		if(_changed.rotation)
			_rotationMatrix = glm::rotate(glm::mat4(1), _rotation.x, glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1), _rotation.y, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), _rotation.z, glm::vec3(0, 0, 1));
		if(_changed.scale)
			_scaleMatrix = glm::scale(glm::mat4(1), _scale);
		if(_changed)
			_modelMatrix = _translationMatrix * _rotationMatrix * _scaleMatrix;

		_changed = false;
	}

public:
	Transform() : _changed({ false, false, false }), _position(0), _rotation(0), _scale(1), _modelMatrix(1) {}
	Transform(glm::vec3 position, glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1)) : _changed({ false, false, false }), _position(position), _rotation(rotation), _scale(scale) {}

	inline const glm::vec3& GetPosition() { return _position; }
	inline const glm::vec3& GetRotation() { return _rotation; }
	inline const glm::vec3& GetScale() { return _scale; }
	inline void SetPosition(const glm::vec3& position) { _position = position; _changed.position = true; }
	inline void SetRotation(const glm::vec3& rotation) { _rotation = rotation; _changed.rotation = true; }
	inline void SetScale(const glm::vec3& scale) { _scale = scale; _changed.scale = true; }

	inline void Translate(glm::vec3 vec) { _position += vec; _changed.position = true; }
	inline void Rotate(glm::vec3 euler) { _rotation += euler; _changed.rotation = true; }
	inline void Scale(glm::vec3 factor) { _scale += factor; _changed.scale = true; }

	inline const glm::mat4& GetTranslationMatrix()
	{
		if (_changed.position)
			_translationMatrix = glm::translate(glm::mat4(1), _position);
		
		return _translationMatrix;
	}
	inline const glm::mat4& GetRotationMatrix()
	{
		if (_changed.rotation)
			_rotationMatrix = glm::rotate(glm::mat4(1), _rotation.x, glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1), _rotation.y, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), _rotation.z, glm::vec3(0, 0, 1));

		return _rotationMatrix;
	}
	inline const glm::mat4& GetScaleMatrix()
	{
		if (_changed.scale)
			_scaleMatrix = glm::scale(glm::mat4(1), _scale);

		return _scaleMatrix;
	}
	inline const glm::mat4& GetMatrix()
	{
		Recalculate();

		return _modelMatrix;
	}
};