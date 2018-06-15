#pragma once

#include <string>
#include <vector>

#include "Face.h"

struct Group
{
	std::string name;
	std::string material;

	std::vector<Face> faces;

	Group(void) {}
	Group(const std::string& name) : name(name) {}
};