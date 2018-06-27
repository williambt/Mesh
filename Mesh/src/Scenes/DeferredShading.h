#pragma once
#include "Scene.h"
#include "../Light/PointLight.h"
#include "../Renderer/IndexBuffer.h"

void renderQuad();
unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

class DeferredShading : public Scene
{
private:

	Shader gBufferShader, lightingPassShader;
	unsigned int gBuffer;
	unsigned int gPosition, gNormal, gColour;
	unsigned int rboDepth;

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

		Mesh mesh1;
		readObj("res/floor.obj", mesh1);

		_objects.push_back(meshToDrawable(mesh1));
		_objects[0]->transform.SetPosition(glm::vec3(0, 0, -1));

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
		//_camera.Rotate(glm::vec3(-30, 0, 0));
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
		glDrawBuffers(3, attachments);

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
