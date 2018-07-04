#pragma once

#include "Curve.h"

class AdvancedCurve : public Curve
{
public:
	glm::vec2 _controlPoints[4];
	glm::vec2 offset;
};