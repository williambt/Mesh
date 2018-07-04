#pragma once

#include <vector>

struct Face
{
	std::vector<unsigned int> verts;
	std::vector<unsigned int> norms;
	std::vector<unsigned int> texts;
};