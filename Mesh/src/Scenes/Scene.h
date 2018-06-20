#pragma once

#include <vector>
#include "../Shader/Shader.h"
#include "../Mesh/Drawable.h"
#include "../Light/Light.h"
#include "../Input/Input.h"
#include "../Window/Window.h"
#include "Camera.h"

class Scene
{
protected:
	std::vector<Drawable*> _objects;
	std::vector<Light*> _lights;

	Camera2 _camera;

	Shader _shader;

public:
	Scene(void) : _shader("res/shaders/basic.vs", "res/shaders/basic.fs"), _camera(glm::vec3(0, 0, 1)) {}//, glm::vec3(0, 0, 0)) {}
	virtual ~Scene(void)
	{
		for (Drawable* d : _objects)
			delete d;
		for (Light* l : _lights)
			delete l;
	}

	virtual void Setup() = 0;
	virtual void Update()
	{
		Input::Update();
		Window::SwapBuffers();
		glfwPollEvents();
	}

	virtual void Draw()
	{
		glClearColor(1.0f, 0.75f, 0.25f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (Drawable* drawable : _objects)
			drawable->Draw(_shader);
	}
};