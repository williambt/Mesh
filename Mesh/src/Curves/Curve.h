#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include "../Renderer/VertexArray.h"

class Curve
{
private:

public:

	std::vector<glm::vec3> ComputePoints(float start, float end, float step)
	{
		std::vector<glm::vec3> points;
		for (float t = start; t <= end+step; t += step)
		{
			points.push_back(Evaluate(t));
		}

		return points;
	}

	glm::vec3 Evaluate(float t)
	{
		return glm::vec3(cosf(t*2*3.14159265), sinf(t * 2 * 3.14159265), 0);
	}

	void Draw(float start, float end, float step)
	{
		Shader s("res/shaders/line.vs", "res/shaders/line.fs");
		s.Bind();
		auto points = ComputePoints(start, end, step);
		VertexBuffer vb(&points[0], points.size() * sizeof(glm::vec3));
		vb.Bind();
		VertexArray va;
		BufferLayout layout;
		layout.AddElement(LayoutElement(3, GL_FLOAT, ATTRIB_VERTEX_POS));
		va.AddBuffers(vb, layout);
		va.Bind();
		glLineWidth(2);
		glDrawArrays(GL_LINE_STRIP, 0, points.size());
	}
};