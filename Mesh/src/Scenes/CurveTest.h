#pragma once

#include "Scene.h"
#include "../Curves/Hermite.h"
#include "../Curves/Bezier.h"
#include "../Curves/BSpline.h"

static void DrawAdvancedCurve(AdvancedCurve& h, Shader& s);

static float remap(float val, float min, float max, float nMin, float nMax)
{
	return (val - min) / (max - min) * (nMax - nMin) + nMin;
}

static glm::vec2 remap(glm::vec2 val, float min, float max, float nMin, float nMax)
{
	return glm::vec2(remap(val.x, min, max, nMin, nMax), remap(val.y, min, max, nMin, nMax));
}

std::ostream& operator <<(std::ostream& stream, glm::vec2 vec)
{
	stream << vec.x << ", " << vec.y;
	return stream;
}

class CurveTest : public Scene
{
private:
	Shader _lineShader;
	Hermite _hermiteCurve;
	Bezier _bezierCurve;
	BSpline _bsplineCurve;
	glm::vec2* selected;

public:

	void Setup()
	{
		_lineShader.Load("res/shaders/line.vs", "res/shaders/line.fs");

		_hermiteCurve.AddPoints(glm::vec3(-0.9f, -0.9f, 0), glm::vec3(0.9f, 0.9f, 0), glm::vec3(0.9f, -0.9f, 0), glm::vec3(-0.9f, 0.9f, 0));
		_bezierCurve.AddPoints(glm::vec3(-0.9f, -0.9f, 0), glm::vec3(-0.9f, 0.9f, 0), glm::vec3(0.9f, 0.9f, 0), glm::vec3(0.9f, -0.9f, 0));
		_bsplineCurve.AddPoints(glm::vec3(-0.9f, -0.9f, 0), glm::vec3(-0.9f, 0.9f, 0), glm::vec3(0.9f, 0.9f, 0), glm::vec3(0.9f, -0.9f, 0));
	}

	void Update()
	{
		if (Input::GetMButtonPressed(GLFW_MOUSE_BUTTON_1))
		{
			glm::vec2 mpos = Input::GetMousePos();
			mpos = remap(mpos, 0, 800, -1, 1);
			mpos.y = -mpos.y;
			for (glm::vec2& p : _bsplineCurve._controlPoints)
			{
				if (glm::length(p - mpos) < 0.10f)
				{
					selected = &p;
					break;
				}
				selected = NULL;
			}
		}
		else if (Input::MButtonState(GLFW_MOUSE_BUTTON_1) == Input::KEY_RELEASED)
		{
			selected = NULL;
		}

		if (selected != NULL)
		{
			glm::vec2 mmov = remap(Input::GetMouseMovement(), -400, 400, -1, 1);
			mmov.y = -mmov.y;
			*selected += mmov;
		}

		Scene::Update();
	}

	void Draw()
	{
		glClearColor(1.0f, 0.75f, 0.25f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		DrawAdvancedCurve(_bsplineCurve, _lineShader);
	}

};

static void DrawAdvancedCurve(AdvancedCurve& h, Shader& s)
{
	std::vector<glm::vec2> points = h.ComputePoints(0.005f);
	s.Bind();

	s.Uniform4f("colour", glm::vec4(1.0f, 0.2f, 0.25f, 1.0f));

	glLineWidth(2);
	glBegin(GL_LINE_STRIP);
	{
		for (int i = 0; i < points.size(); ++i)
			glVertex2f(points[i].x, points[i].y);
	}
	glEnd();

	s.Uniform4f("colour", glm::vec4(0.0f, 1.0f, 0.33f, 1.0f));

	glPointSize(10);
	glBegin(GL_POINTS);
	{
		for (glm::vec2 p : h._controlPoints)
		{
			glVertex2f(p.x, p.y);
		}
	}
	glEnd();

}