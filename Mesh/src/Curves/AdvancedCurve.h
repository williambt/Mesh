#pragma once

#include "Curve.h"

class AdvancedCurve : public Curve
{
public:
	std::vector<glm::vec2> _controlPoints;
};