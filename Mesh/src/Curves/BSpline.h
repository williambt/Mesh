#pragma once

#include "AdvancedCurve.h"
#include <glm/vec2.hpp>

class BSpline : public AdvancedCurve
{
public:

	glm::vec2 Evaluate(float t)
	{
		glm::vec2 res;

		res.x = (-powf(t, 3) + 3 * powf(t, 2) - 3 * t + 1) * _controlPoints[0].x;
		res.x += (3 * powf(t, 3) - 6 * powf(t, 2) + 4) * _controlPoints[1].x;
		res.x += (-3 * powf(t, 3) + 3 * powf(t, 2) + 3*t + 1) * _controlPoints[2].x;
		res.x += powf(t, 3)*_controlPoints[3].x;

		res.y = (-powf(t, 3) + 3 * powf(t, 2) - 3 * t + 1) * _controlPoints[0].y;
		res.y += (3 * powf(t, 3) - 6 * powf(t, 2) + 4) * _controlPoints[1].y;
		res.y += (-3 * powf(t, 3) + 3 * powf(t, 2) + 3 * t + 1) * _controlPoints[2].y;
		res.y += powf(t, 3)*_controlPoints[3].y;

		res.x = res.x / 6.0f;
		res.y = res.y / 6.0f;

		return res;
	}

	void AddPoints(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4)
	{
		_controlPoints.push_back(p1);
		_controlPoints.push_back(p2);
		_controlPoints.push_back(p3);
		_controlPoints.push_back(p4);
	}

};