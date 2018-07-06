#pragma once

#include "Scene.h"
#include "../Curves/Hermite.h"
#include "../Curves/Bezier.h"
#include "../Curves/BSpline.h"
#include "../Mesh/Writer.h"
#include <vector>

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

	bool firstCreated = false;
	glm::vec2 firstPoint;

	std::vector<Bezier> _bezierCurves;
	Bezier* selectedCurve;
	int selectedIndex;

	bool drawPointsAndHull = true;
	bool drawInternalAndExternal = false;

	bool showCompleted = true;

public:

	void Setup()
	{
		_lineShader.Load("res/shaders/line.vs", "res/shaders/line.fs");
		_bezierCurves.reserve(100);
		//_bezierCurves.push_back(Bezier());
		//_bezierCurves.push_back(Bezier());
		//_bezierCurves[0].AddPoints(glm::vec3(-0.9f, -0.9f, 0), glm::vec3(-0.9f, 0.9f, 0), glm::vec3(0.0f, 0.9f, 0), glm::vec3(0.0f, -0.9f, 0));
		//_bezierCurves[1].AddPoints(glm::vec3(-0.0f, -0.9f, 0), glm::vec3(-0.0f, 0.9f, 0), glm::vec3(0.9f, 0.9f, 0), glm::vec3(0.9f, -0.9f, 0));
		//_bezierCurves[0].AddKnot(&_bezierCurves[1]);
	}

	void Update()
	{
		if (Input::GetMButtonPressed(GLFW_MOUSE_BUTTON_1))
		{
			glm::vec2 mpos = Input::GetMousePos();
			mpos = remap(mpos, 0, 800, -1, 1);
			mpos.y = -mpos.y;
			bool found = false;
			for (Bezier& b : _bezierCurves)
			{
				for (int i = 0; i < 4/*b._controlPoints.size()*/; ++i)
				{
					if (glm::length(b._controlPoints[i] - mpos) < 20.0f / 800.0f)
					{
						selectedCurve = &b;
						selectedIndex = i;
						found = true;
						break;
					}
				}
			}
			if (!found)
			{
				selectedCurve = NULL;

				if (!firstCreated)
				{
					firstCreated = true;
					firstPoint = mpos;
				}
				else
				{
					_bezierCurves.push_back(Bezier());
					_bezierCurves.back().AddPoints(firstPoint, (firstPoint) + (mpos - firstPoint) / 3.0f, (firstPoint)+((mpos - firstPoint) / 3.0f) * 2.0f, mpos);
					if (_bezierCurves.size() > 1)
						(_bezierCurves.end() - 2)->AddKnot(&_bezierCurves.back());
					firstPoint = mpos;
				}
			}
		}
		else if (Input::MButtonState(GLFW_MOUSE_BUTTON_1) == Input::KEY_RELEASED)
		{
			selectedCurve = NULL;
		}

		if (selectedCurve != NULL)
		{
			glm::vec2 mmov = remap(Input::GetMouseMovement(), -400, 400, -1, 1);
			mmov.y = -mmov.y;
			selectedCurve->MovePoint(selectedIndex, mmov);
		}

		if (Input::GetKeyPressed(GLFW_KEY_SPACE))
			drawPointsAndHull = !drawPointsAndHull;

		if (Input::GetKeyDown(GLFW_KEY_LEFT_CONTROL) && Input::GetKeyPressed(GLFW_KEY_Z))
		{
			if(_bezierCurves.size() > 0)
				_bezierCurves.pop_back();
			if (_bezierCurves.size() > 0)
				_bezierCurves.back().RemoveKnot();
		}

		if (Input::GetKeyPressed(GLFW_KEY_Z) && !Input::GetKeyDown(GLFW_KEY_LEFT_CONTROL))
		{
			drawInternalAndExternal = !drawInternalAndExternal;
		}

		if (Input::GetKeyPressed(GLFW_KEY_ENTER))
		{
			FinishCurve();
		}

		if (Input::GetKeyPressed(GLFW_KEY_ESCAPE))
			Window::Close();

		if (Input::GetKeyPressed(GLFW_KEY_C))
		{
			showCompleted = !showCompleted;
		}

		Scene::Update();
	}

	void Draw()
	{
		glClearColor(1.0f, 0.75f, 0.25f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (_bezierCurves.size() > 0)
		{
			_bezierCurves[0].Draw(_lineShader, drawPointsAndHull, drawPointsAndHull);
			if (drawInternalAndExternal)
				DrawInternalAndExternal();

			if (showCompleted)
			{
				std::vector<glm::vec2> points = _bezierCurves[0].ComputePoints(0.7f, true);
				_lineShader.Uniform4f("colour", glm::vec4(1, 0, 0, 1));
				glLineWidth(20);
				glBegin(GL_LINE_STRIP);
				{
					glVertex2f(points[0].x, points[0].y);
					glVertex2f(points.back().x, points.back().y);
				}
				glEnd();
			}
		}
	}

	std::vector<glm::vec2> GetPointsBetween(glm::vec2 p1, glm::vec2 p2, float step)
	{
		glm::vec2 vec = p2 - p1;
		std::vector<glm::vec2> res;
		for (float i = 0; i < 1.0f; i += step)
		{
			glm::vec2 point = p1 + (vec * i);
			res.push_back(point);
		}

		return res;
	}

	std::vector<glm::vec2> CalculateCurve(bool internal)
	{
		std::vector<glm::vec2> points = _bezierCurves[0].ComputePoints(0.05f, true);
		std::vector<glm::vec2> curve;

		std::vector<glm::vec2> lastToFirst = GetPointsBetween(points.back(), points[0], 0.05f);

		auto* current = &points;

		for (int i = 0; i < current->size() - 1; ++i)
		{
			glm::vec2 c;
			glm::vec2 a = current->at(i);
			glm::vec2 b = current->at(i+1);
			glm::vec2 d(b.x, a.y);
			d = glm::normalize(d);

			float w = b.x - a.x;
			float h = b.y - a.y;
			float m = .05f;
			float adAngle = atanf(h / w);
			float angle;
			if (w < 0)
			{
				if (internal)
					angle = adAngle - (3.14159265f / 2.0f);
				else
					angle = adAngle + (3.14159265f / 2.0f);
			}
			else
			{
				if (internal)
					angle = adAngle + (3.14159265f / 2.0f);
				else
					angle = adAngle - (3.14159265f / 2.0f);
			}

			c.x = cosf(angle) * m + a.x;
			c.y = sinf(angle) * m + a.y;
			curve.push_back(c);
			if (current == &points && i == current->size() - 2)
			{
				current = &lastToFirst;
				i = -1;
			}
		}

		curve.push_back(curve.front());

		return curve;
	}
	void FinishCurve()
	{
		auto internalCurve = CalculateCurve(true);
		auto externalCurve = CalculateCurve(false);

		Mesh mesh;
		mesh.groups.push_back(Group("Main"));
		Group& g = mesh.groups[0];
		mesh.textureCoords.push_back(glm::vec2(0, 1));
		mesh.textureCoords.push_back(glm::vec2(1, 1));
		mesh.textureCoords.push_back(glm::vec2(1, 0));
		mesh.textureCoords.push_back(glm::vec2(0, 0));
		mesh.normals.push_back(glm::vec3(0, 1, 0));
		for (int i = 0; i < internalCurve.size(); ++i)
		{
			mesh.vertices.push_back(glm::vec3(internalCurve[i].x, 0, internalCurve[i].y));
			mesh.vertices.push_back(glm::vec3(externalCurve[i].x, 0, externalCurve[i].y));
		}
		for (int i = 0; i < mesh.vertices.size(); i+=2)
		{
			Face f;
			f.verts.push_back(i);
			if (i + 2 < mesh.vertices.size())
			{
				f.verts.push_back(i + 2);
				f.verts.push_back(i + 3);
			}
			else
			{
				f.verts.push_back(0);
				f.verts.push_back(1);
			}
			f.verts.push_back(i + 1);

			f.norms.push_back(0);
			f.norms.push_back(0);
			f.norms.push_back(0);
			f.norms.push_back(0);

			f.texts.push_back(0);
			f.texts.push_back(1);
			f.texts.push_back(2);
			f.texts.push_back(3);

			g.faces.push_back(f);
		}

		writeObj("Pistas/pista.obj", mesh);
	}

	void DrawInternalAndExternal()
	{
		auto internalCurve = CalculateCurve(true);
		auto externalCurve = CalculateCurve(false);

		_lineShader.Uniform4f("colour", glm::vec4(1, .5f, 0, 1));
		glLineWidth(10);
		glBegin(GL_LINE_STRIP);
		{
			for (int i = 0; i < internalCurve.size(); ++i)
				glVertex2f(internalCurve[i].x, internalCurve[i].y);
		}
		glEnd();
		_lineShader.Uniform4f("colour", glm::vec4(0, .5f, 1, 1));
		glBegin(GL_LINE_STRIP);
		{
			for (int i = 0; i < externalCurve.size(); ++i)
				glVertex2f(externalCurve[i].x, externalCurve[i].y);
		}
		glEnd();
	}
};

static void DrawAdvancedCurve(AdvancedCurve& h, Shader& s)
{
	std::vector<glm::vec2> points = h.ComputePoints(0.01f);
	s.Bind();

	s.Uniform4f("colour", glm::vec4(1.0f, 0.2f, 0.25f, 1.0f));

	glLineWidth(4);
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

	s.Uniform4f("colour", glm::vec4(0.0f, 0.2f, 1.0f, 1.0f));

	glLineWidth(2);
	glBegin(GL_LINE_STRIP);
	{
		for (glm::vec2 p : h._controlPoints)
		{
			glVertex2f(p.x, p.y);
		}
	}
	glEnd();
}