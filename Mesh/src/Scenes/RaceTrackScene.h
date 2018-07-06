#pragma once
#include "Scene.h"

class RaceTrackScene : public Scene
{
private:

	Drawable* car;

	float trackScale = 10.0f;
	std::vector<glm::vec3> midPoints;

	float cameraSpeed = 0.25f;
	float lookSensitivity = 0.2f;
	bool cursorLocked = false;
	
	int currPoint = 1;
	float currFactor = 0;

	float carSpeed = 0;
	float carMaxSpeed = 0.2f;
	float acceleration = -0.02f;

	bool playing = false;

public:

	void Setup()
	{
		Mesh track;
		Mesh carMesh;
		Mesh sceneMesh;
		readObj("Pistas/pista.obj", track);
		readObj("res/car.obj", carMesh);
		readObj("res/Scene.obj", sceneMesh);
		_objects.push_back(meshToDrawable(sceneMesh));
		_objects.back()->transform.SetPosition(glm::vec3(0, -0.1f, 0));

		for (int i = 0; i < track.vertices.size(); i += 2)
		{
			glm::vec3 p1 = track.vertices[i];
			glm::vec3 p2 = track.vertices[i + 1];
			midPoints.push_back((p1 + ((p2 - p1)/2.0f)) * trackScale);
		}

		car = meshToDrawable(carMesh);
		_objects.push_back(car);
		car->transform.SetPosition(midPoints[0]);

		_objects.push_back(meshToDrawable(track));
		_objects.back()->transform.SetScale(glm::vec3(trackScale));
		
		PointLight* l1 = new PointLight(glm::vec3(-5, 3, -5));
		l1->constantAttenuation = 0.33f;
		_lights.push_back(l1);

		PointLight* l2 = new PointLight(glm::vec3(5, 3, -5));
		l2->constantAttenuation = 0.33f;
		_lights.push_back(l2);

		PointLight* l3 = new PointLight(glm::vec3(5, 3, 5));
		l3->constantAttenuation = 0.33f;
		_lights.push_back(l3);

		PointLight* l4 = new PointLight(glm::vec3(-5, 3, 5));
		l4->constantAttenuation = 0.33f;
		_lights.push_back(l4);

		//_lights.push_back(new Light(glm::vec3(0, 1, 0)));
		_shader.Uniform1i("lightNumber", _lights.size());
		for (unsigned int i = 0; i < _lights.size(); ++i) //Apply lights to shader
		{
			_lights[i]->ApplyToShader(_shader, i);
		}

		glm::mat4 proj = glm::perspective(glm::radians(85.0f), (float)Window::GetWidth() / (float)Window::GetHeight(), 0.001f, 1000.0f);
		_shader.UniformMat4f("pMat", proj);

		_camera.SetPosition(glm::vec3(0, 2, 5));
	}

	void Update()
	{
		_shader.UniformMat4f("vMat", _camera.GetMatrix());

		if (Input::GetKeyDown(GLFW_KEY_W))
		{
			_camera.Translate(_camera.GetForward() * cameraSpeed);
		}
		if (Input::GetKeyDown(GLFW_KEY_S))
		{
			_camera.Translate(_camera.GetForward() * -cameraSpeed);
		}
		if (Input::GetKeyDown(GLFW_KEY_D))
		{
			_camera.Translate(_camera.GetRight() * cameraSpeed);
		}
		if (Input::GetKeyDown(GLFW_KEY_A))
		{
			_camera.Translate(_camera.GetRight() * -cameraSpeed);
		}

		glm::vec2 mmove = Input::GetMouseMovement() * lookSensitivity;
		_camera.Rotate(glm::vec3(-mmove.y, mmove.x, 0));

		if (Input::GetKeyPressed(GLFW_KEY_F1))
		{
			if (cursorLocked)
				Input::UnlockCursor();
			else
				Input::LockCursor();
			cursorLocked = !cursorLocked;
		}
		if (Input::GetKeyPressed(GLFW_KEY_ESCAPE))
			Window::Close();

		if (Input::GetKeyPressed(GLFW_KEY_SPACE))
		{
			if (!playing)
				playing = true;
			acceleration = -acceleration;
		}

		if (playing)
		{
			if (currFactor > 1.0f)
			{
				currFactor = 0;
				currPoint++;
				if (currPoint >= midPoints.size())
					currPoint = 0;
			}
			int previousPoint = currPoint - 1;;
			if (previousPoint < 0)
				previousPoint = midPoints.size() - 1;
			car->transform.SetPosition(midPoints[previousPoint] + ((midPoints[currPoint] - midPoints[previousPoint]) * currFactor));

			currFactor += carSpeed / glm::length(midPoints[currPoint] - midPoints[previousPoint]);
			carSpeed += acceleration;
			if (carSpeed > carMaxSpeed)
				
				carSpeed = carMaxSpeed;

			if (carSpeed <= 0.0f)
			{
				playing = false;
				carSpeed = 0.0f;
			}

			glm::vec2 a = glm::vec2(midPoints[previousPoint].x, midPoints[previousPoint].z);
			glm::vec2 b = glm::vec2(midPoints[currPoint].x, midPoints[currPoint].z);
			float angle = glm::acos(glm::dot(glm::normalize(a - b), glm::vec2(1, 0)));
			float w = b.x - a.x;
			float h = b.y - a.y;
			if (w < 0.0f)
				angle += 3.14159265f;
			car->transform.SetRotation(glm::vec3(0, angle, 0));
		}

		Scene::Update();
	}

	void Draw()
	{
		glClearColor(0.10f, 0.55f, 0.86f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		for (Drawable* drawable : _objects)
			drawable->Draw(_shader);
	}
};