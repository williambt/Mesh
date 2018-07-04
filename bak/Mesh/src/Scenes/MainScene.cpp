#include "../Window/Window.h"
#include "MainScene.h"
#include "../Light/PointLight.h"

void MainScene::Setup()
{
	Mesh m1, m2; 
	readObj("res/sphere.obj", m1);
	readObj("res/teapot.obj", m2);

	_objects.push_back(meshToDrawable(m1));
	_objects.push_back(meshToDrawable(m2));

	d = _objects[0];

	PointLight* light = new PointLight(glm::vec3(0, 0, 0));
	light->constantAttenuation = 0.3f;

	_lights.push_back(new Light(glm::vec3(1, 0, 1), glm::vec4(0.75f, 0.25f, 0.25f, 1)));
	_lights.push_back(new Light(glm::vec3(-1, 0, 1), glm::vec4(0.25f, 0.25f, 0.75f, 1)));
	_shader.Uniform1i("lightNumber", _lights.size());
	for (unsigned int i = 0; i < _lights.size(); ++i)
	{
		_lights[i]->ApplyToShader(_shader, i);
	}

	glm::mat4 proj = glm::perspective(glm::radians(75.0f), 1.0f, 0.001f, 1000.0f);
	_shader.UniformMat4f("pMat", proj);
}

void MainScene::Update()
{
	float deltaTime = glfwGetTime() - lastTime;
	lastTime = glfwGetTime();

	secondCounter += deltaTime;
	++fps;
	if (secondCounter >= 1.0f)
	{
		std::cout << "FPS: " << fps << std::endl;
		fps = 0;
		secondCounter -= 1.0f;
	}

	if (Input::GetKeyPressed(GLFW_KEY_SPACE))
	{
		if (++currentObjIndex >= (int)_objects.size())
			currentObjIndex = 0;
		d = _objects[currentObjIndex];
	}

	//ROTATION
	if (Input::GetKeyDown(GLFW_KEY_LEFT))
	{
		d->transform.Rotate(glm::vec3(0, -rotateSpeed, 0));
	}
	if (Input::GetKeyDown(GLFW_KEY_RIGHT))
	{
		d->transform.Rotate(glm::vec3(0, rotateSpeed, 0));
	}
	if (Input::GetKeyDown(GLFW_KEY_UP))
	{
		d->transform.Rotate(glm::vec3(-rotateSpeed, 0, 0));
	}
	if (Input::GetKeyDown(GLFW_KEY_DOWN))
	{
		d->transform.Rotate(glm::vec3(rotateSpeed, 0, 0));
	}

	//TRANSLATION
	if (Input::GetKeyDown(GLFW_KEY_W))
	{
		d->transform.Translate(glm::vec3(0, moveSpeed, 0));
	}
	if (Input::GetKeyDown(GLFW_KEY_S))
	{
		d->transform.Translate(glm::vec3(0, -moveSpeed, 0));
	}
	if (Input::GetKeyDown(GLFW_KEY_A))
	{
		d->transform.Translate(glm::vec3(-moveSpeed, 0, 0));
	}
	if (Input::GetKeyDown(GLFW_KEY_D))
	{
		d->transform.Translate(glm::vec3(moveSpeed, 0, 0));
	}

	if (Input::GetKeyDown(GLFW_KEY_R))
	{
		d->transform.Translate(glm::vec3(0, 0, -moveSpeed));
	}
	if (Input::GetKeyDown(GLFW_KEY_F))
	{
		d->transform.Translate(glm::vec3(0, 0, moveSpeed));
	}

	if (Input::GetKeyPressed(GLFW_KEY_ESCAPE))
		Window::Close();

	Scene::Update();
}