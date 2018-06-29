#pragma once
#include "AdvancedCurve.h"
#include <glm/vec2.hpp>

class Bezier : public AdvancedCurve
{
private:
	Bezier* next;
	Bezier* previous;

public:

	Bezier() : next(NULL), previous(NULL), AdvancedCurve() {}
	
	glm::vec2 Evaluate(float t)
	{
		glm::vec2 res;

		res.x = (-powf(t, 3) + 3 * powf(t, 2) - 3 * t + 1) * _controlPoints[0].x;
		res.x += (3 * powf(t, 3) - 6 * powf(t, 2) + 3 * t) * _controlPoints[1].x;
		res.x += (-3 * powf(t, 3) + 3 * powf(t, 2)) * _controlPoints[2].x;
		res.x += powf(t, 3)*_controlPoints[3].x;

		res.y = (-powf(t, 3) + 3 * powf(t, 2) - 3 * t + 1) * _controlPoints[0].y;
		res.y += (3 * powf(t, 3) - 6 * powf(t, 2) + 3 * t) * _controlPoints[1].y;
		res.y += (-3 * powf(t, 3) + 3 * powf(t, 2)) * _controlPoints[2].y;
		res.y += powf(t, 3)*_controlPoints[3].y;

		return res;
	}

	void AddPoints(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4)
	{
		/*_controlPoints.push_back(p1);
		  _controlPoints.push_back(p2);
		  _controlPoints.push_back(p3);
		  _controlPoints.push_back(p4);*/

		_controlPoints[0]=(p1);
		_controlPoints[1]=(p2);
		_controlPoints[2]=(p3);
		_controlPoints[3]=(p4);
	}

	void AddKnot(Bezier* other)
	{
		next = other;
		other->previous = this;
		other->_controlPoints[0] = _controlPoints[3];
	}

	glm::vec2 GetPoint(int index) 
	{ 
		if (index < 4 && index >= 0)
		{
			return _controlPoints[index];
		}
		else
			return glm::vec2(0);
	}
	void SetPoint(int index, glm::vec2 point) 
	{ 
		if (index < 4 && index >= 0)
		{
			if (index == 3 && next)
			{
				_controlPoints[3] = point;
				next->_controlPoints[0] = point;
			}
			else if (index == 0 && previous)
			{
				_controlPoints[0] = point;
				previous->_controlPoints[3] = point;
			}
			else
			{
				_controlPoints[index] = point;
			}
		}
	}

	void MovePoint(int index, glm::vec2 factor)
	{
		SetPoint(index, GetPoint(index) + factor);
	}

	void Draw(const Shader& s, bool drawPoints = true, bool drawHull = true, bool drawAllAttached = true)
	{
		std::vector<glm::vec2> points = ComputePoints(0.01f);
		s.Bind();

		s.Uniform4f("colour", glm::vec4(1.0f, 0.2f, 0.25f, 1.0f));

		glLineWidth(4);
		glBegin(GL_LINE_STRIP);
		{
			for (int i = 0; i < points.size(); ++i)
				glVertex2f(points[i].x, points[i].y);
		}
		glEnd();

		if (drawPoints)
		{
			s.Uniform4f("colour", glm::vec4(0.0f, 1.0f, 0.33f, 1.0f));

			glPointSize(10);
			glBegin(GL_POINTS);
			{
				for (glm::vec2 p : _controlPoints)
				{
					glVertex2f(p.x, p.y);
				}
			}
			glEnd();
		}
		if (drawHull)
		{
			s.Uniform4f("colour", glm::vec4(0.0f, 0.2f, 1.0f, 1.0f));

			glLineWidth(2);
			glBegin(GL_LINE_STRIP);
			{
				for (glm::vec2 p : _controlPoints)
				{
					glVertex2f(p.x, p.y);
				}
			}
			glEnd();
		}
		if (drawAllAttached && next)
			next->Draw(s, drawPoints, drawHull, true);
	}
};