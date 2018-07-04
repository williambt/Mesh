#pragma once
#include "Scene.h"
#include "../Light/PointLight.h"
#include "../Renderer/IndexBuffer.h"

void renderQuad();
unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

class DeferredShading : public Scene
{
private:

	Shader gBufferShader, lightingPassShader, lineShader;
	Drawable* room;
	unsigned int gBuffer;
	unsigned int gPosition, gNormal, gColour;
	unsigned int rboDepth;
	float moveSpeed = 0.25f;
	float lookSensitivity = 0.2f;
	bool cursorLocked = false;

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
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

		glGenTextures(1, &gColour);
		glBindTexture(GL_TEXTURE_2D, gColour);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Window::GetWidth(), Window::GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColour, 0);

		glDrawBuffers(3, attachments);

		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 800);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer nor complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		gBufferShader.Load("res/shaders/gBuffer.vs", "res/shaders/gBuffer.fs");
		lightingPassShader.Load("res/shaders/lightingPass.vs", "res/shaders/lightingPass.fs");
		lineShader.Load("res/shaders/line.vs", "res/shaders/line.fs");

		Mesh mesh1;
		readObj("res/thing.obj", mesh1);

		Mesh roomMesh;
		readObj("res/cube.obj", roomMesh);
		
		room = meshToDrawable(roomMesh);
		room->transform.Scale(glm::vec3(10, 10, 10));

		_objects.push_back(meshToDrawable(mesh1));
		_objects[0]->transform.SetPosition(glm::vec3(0, 0, -1));

		_objects.push_back(meshToDrawable(mesh1));
		_objects[1]->transform.SetPosition(glm::vec3(-2, 0, -1));

		_objects.push_back(meshToDrawable(mesh1));
		_objects[2]->transform.SetPosition(glm::vec3(2, 0, -1));

		_objects.push_back(meshToDrawable(mesh1));
		_objects[3]->transform.SetPosition(glm::vec3(0, 0, -3));

		PointLight* pl1 = new PointLight(glm::vec3(-3, 2.0f, 0), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
		pl1->constantAttenuation = 0.33f;

		PointLight* pl2 = new PointLight(glm::vec3(3, 2.0f, 0), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
		pl2->constantAttenuation = 0.33f;

		_lights.push_back(pl1);
		_lights.push_back(pl2);
		lightingPassShader.Uniform1i("lightNumber", _lights.size());
		_shader.Uniform1i("lightNumber", _lights.size());
		for (unsigned int i = 0; i < _lights.size(); ++i)
		{
			_lights[i]->ApplyToShader(lightingPassShader, i);
		}
		for (unsigned int i = 0; i < _lights.size(); ++i)
		{
			_lights[i]->ApplyToShader(_shader, i);
		}

		glm::mat4 proj = glm::perspective(glm::radians(85.0f), (float)Window::GetWidth() / (float)Window::GetHeight(), 0.001f, 1000.0f);
		gBufferShader.UniformMat4f("pMat", proj);
		_shader.UniformMat4f("pMat", proj);

		_camera.SetPosition(glm::vec3(0.0f, 1.5f, 1.0f));
	}

	void Update()
	{
		if (Input::GetKeyDown(GLFW_KEY_W))
		{
			_camera.Translate(_camera.GetForward() * moveSpeed);
		}
		if (Input::GetKeyDown(GLFW_KEY_S))
		{
			_camera.Translate(_camera.GetForward() * -moveSpeed);
		}
		if (Input::GetKeyDown(GLFW_KEY_D))
		{
			_camera.Translate(_camera.GetRight() * moveSpeed);
		}
		if (Input::GetKeyDown(GLFW_KEY_A))
		{
			_camera.Translate(_camera.GetRight() * -moveSpeed);
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

		Scene::Update();
	}

	void BindGBufferTextures()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gColour);

		lightingPassShader.Uniform1i("gPosition", 0);
		lightingPassShader.Uniform1i("gNormal", 1);
		lightingPassShader.Uniform1i("gColour", 2);
	}

	virtual void Draw()
	{
		glClearColor(1.0f, 0.75f, 0.25f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

		glClearBufferfv(GL_COLOR, 0, new float[3] {0.0f, 0.0f, -10000.0f});

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gBufferShader.Bind();
		gBufferShader.UniformMat4f("vMat", _camera.GetMatrix());

		for (Drawable* drawable : _objects)
			drawable->Draw(gBufferShader);


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		lightingPassShader.Bind();
		lightingPassShader.Uniform3f("viewPos", _camera.GetPosition());

		BindGBufferTextures();

		renderQuad();

		glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, Window::GetWidth(), Window::GetHeight(), 0, 0, Window::GetWidth(), Window::GetHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		_shader.Bind();
		_shader.UniformMat4f("vMat", _camera.GetMatrix());

		room->Draw(_shader);
	}
};

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
