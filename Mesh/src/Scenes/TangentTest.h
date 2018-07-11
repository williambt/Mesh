#pragma once

#include "Scene.h"
#include <glm/vec3.hpp>
#include "../Light/PointLight.h"

class TangentTest : public Scene
{
private:
	Shader s;
	
	glm::vec3 pos[4] =
	{
		glm::vec3(-1.0,  1.0, 0.0),
		glm::vec3(-1.0, -1.0, 0.0),
		glm::vec3(1.0, -1.0, 0.0),
		glm::vec3(1.0,  1.0, 0.0)
	};

	glm::vec2 uv[4] =
	{
		glm::vec2(0.0, 1.0),
		glm::vec2(0.0, 0.0),
		glm::vec2(1.0, 0.0),
		glm::vec2(1.0, 1.0)
	};

	glm::vec3 normal = glm::vec3(0.0, 0.0, 1.0);

	float cameraSpeed = 0.25f;
	float lookSensitivity = 0.2f;
	bool cursorLocked = false;

	std::vector<Material*> mats;

public:


	void Setup()
	{
		s.Load("res/shaders/normalmapping.vs", "res/shaders/normalmapping.fs");
		Mesh nanosuitMesh;
		readObj("res/orange/orange.obj", nanosuitMesh);
		_objects.push_back(meshToDrawable(nanosuitMesh));
		for (DrawableGroup* dg : _objects[0]->groups)
		{
			mats.push_back(&dg->material);
		}

		PointLight* pl = new PointLight(glm::vec3(0, 2, 5));
		pl->constantAttenuation = 0.1f;
		_lights.push_back(pl);

		s.Uniform1i("lightNumber", _lights.size());
		for (int i = 0; i < _lights.size(); ++i)
		{
			_lights[i]->ApplyToShader(s, i);
		}

		s.UniformMat4f("pMat", glm::perspective(glm::radians(75.0f), 1.0f, 0.01f, 1000.0f));
		glm::mat4 transform = glm::scale(glm::mat4(1), glm::vec3(10, 10, 1));

		_camera.SetPosition(glm::vec3(0, 0, 2));
	}

	void Update()
	{
		s.UniformMat4f("vMat", _camera.GetMatrix());

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
			for (Material* mat : mats)
			{
				mat->hasNormalMap = !mat->hasNormalMap;
			}
		}

		for (int i = 0; i < _lights.size(); ++i)
		{
			_lights[i]->position = _camera.GetPosition();
			_lights[i]->ApplyToShader(s, i);
		}

		Scene::Update();
	}

	void Draw()
	{
		glClearColor(0.25f, 0.33f, 0.75f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (Drawable* d : _objects)
			d->Draw(s);
	}

};