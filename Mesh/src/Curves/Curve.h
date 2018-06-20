#pragma once

#include <glm/vec3.hpp>
#include <vector>

class Curve
{
public:
	virtual glm::vec2 Evaluate(float t) = 0;
	/*{

	}*/

	std::vector<glm::vec2> ComputePoints(float step)
	{
		std::vector<glm::vec2> points;
		for (float i = 0.0f; i < 1.0f; i += step)
		{
			if (i == 0.999f)
				i = 1;

			points.push_back(Evaluate(i));
			if (i + step > 1.0f && i < 0.999f)
				i = 0.999f;
		}
		return points;
	}
};