#pragma once
#include "Scene.h"
#include "../Light/PointLight.h"

class DeferredShading : public Scene
{
private:

	Shader gBufferShader, lightingPassShader;
	unsigned int gBuffer;
	unsigned int gPosition, gNormal, gColour;

public:

	virtual void Setup()
	{
		glGenFramebuffers(1, &gBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

		glGenTextures(1, &gPosition);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Window::GetWidth(), Window::GetHeight(), 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

		glGenTextures(1, &gNormal);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Window::GetWidth(), Window::GetHeight(), 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gNormal, 0);

		glGenTextures(1, &gColour);
		glBindTexture(GL_TEXTURE_2D, gColour);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Window::GetWidth(), Window::GetHeight(), 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gColour, 0);

		gBufferShader.Load("res/shaders/gBuffer.vs", "res/shaders/gBuffer.fs");
		lightingPassShader.Load("res/shaders/lightingPass.vs", "res/shaders/lightingPass.fs");

		Mesh mesh1;
		readObj("res/floor.obj", mesh1);

		_objects.push_back(meshToDrawable(mesh1));

		PointLight* pl = new PointLight(glm::vec3(0, 2.0f, 0));
		pl->constantAttenuation = 0.33f;

		_lights.push_back(pl);
		lightingPassShader.Uniform1i("lightNumber", _lights.size());
		for (unsigned int i = 0; i < _lights.size(); ++i)
		{
			_lights[i]->ApplyToShader(lightingPassShader, i);
		}

		glm::mat4 proj = glm::perspective(glm::radians(85.0f), (float)Window::GetWidth() / (float)Window::GetHeight(), 0.001f, 1000.0f);
		gBufferShader.UniformMat4f("pMat", proj);

		_camera.SetPosition(glm::vec3(0.0f, 1.5f, 1.0f));
		_camera.Rotate(glm::vec3(30, 0, 0));
	}

	virtual void Draw()
	{
		glClearColor(1.0f, 0.75f, 0.25f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

		for (Drawable* drawable : _objects)
			drawable->Draw(_shader);
	}
};