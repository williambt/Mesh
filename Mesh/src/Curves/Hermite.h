#pragma once

#include "AdvancedCurve.h"
#include <glm/vec2.hpp>

class Hermite : public AdvancedCurve
{
private:
	float _WG = 1.0f;

public:

	glm::vec2 Evaluate(float t)
	{
		glm::vec2 vec;
		vec.x = ((2 * powf(t, 3) - 3 * powf(t, 2) + 0 * t + 1)*_controlPoints[0].x +
			(-2 * powf(t, 3) + 3 * powf(t, 2) + 0 * t + 0)*_controlPoints[1].x +
			(1 * powf(t, 3) - 2 * powf(t, 2) + 1 * t + 0)*_controlPoints[2].x * _WG +
			(1 * powf(t, 3) - 1 * powf(t, 2) + 0 * t + 0)*_controlPoints[3].x * _WG);
		vec.y = ((2 * powf(t, 3) - 3 * powf(t, 2) + 0 * t + 1)*_controlPoints[0].y +
			(-2 * powf(t, 3) + 3 * powf(t, 2) + 0 * t + 0)*_controlPoints[1].y +
			(1 * powf(t, 3) - 2 * powf(t, 2) + 1 * t + 0)*_controlPoints[2].y * _WG +
			(1 * powf(t, 3) - 1 * powf(t, 2) + 0 * t + 0)*_controlPoints[3].y * _WG);
		return vec;
	}

	void AddPoints(glm::vec2 p1, glm::vec2 p2, glm::vec2 r1, glm::vec2 r2)
	{
		/*_controlPoints.push_back(p1);
		_controlPoints.push_back(p2);
		_controlPoints.push_back(r1);
		_controlPoints.push_back(r2);*/
	}

};