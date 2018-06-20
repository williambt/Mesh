#include "../Window/Window.h"
#include "MainScene.h"
#include "../Light/PointLight.h"

static void DrawHermite(Hermite& h, Shader& s);

int selected = 0;

void MainScene::Setup()
{
	//Load Mesh
	Mesh m1, m2; 
	readObj("res/floor.obj", m1);
	//readObj("res/teapot.obj", m2);

	//Add convert mesh to drawable and add to scene
	_objects.push_back(meshToDrawable(m1));
	//_objects[0]->transform.SetScale(glm::vec3(0.33f));
	//_objects.push_back(meshToDrawable(m2));

	d = _objects[0];

	//Add lights to scene
	PointLight* light = new PointLight(glm::vec3(0, 0, 0));
	light->constantAttenuation = 0.3f;

	_lights.push_back(new PointLight(glm::vec3(1, 1.5f, -0.2f), glm::vec4(1, 0.75f, 0.75f, 1)));
	//_lights.push_back(new PointLight(glm::vec3(-1, 0, 2.5f), glm::vec4(0.25f, 0.25f, 0.75f, 1)));
	_shader.Uniform1i("lightNumber", _lights.size());
	for (unsigned int i = 0; i < _lights.size(); ++i) //Apply lights to shader
	{
		_lights[i]->ApplyToShader(_shader, i);
	}

	//Apply projection to shader
	glm::mat4 proj = glm::perspective(glm::radians(85.0f), (float)Window::GetWidth() / (float)Window::GetHeight() , 0.001f, 1000.0f);
	_shader.UniformMat4f("pMat", proj);

	_camera.SetPosition(glm::vec3(0, 2, 5));

	/*h.AddPoints(glm::vec2(-0.8, -0.8), glm::vec2(0.8, 0.8), glm::vec2(1, 0), glm::vec2(10, 0));
	lineShader.Load("res/shaders/line.vs", "res/shaders/line.fs");*/
}

void MainScene::Update()
{
	float deltaTime = glfwGetTime() - lastTime;
	lastTime = glfwGetTime();

	secondCounter += deltaTime;
	++fps;
	if (secondCounter >= 1.0f)
	{
		std::string title = "FPS: " + std::to_string(fps);
		Window::SetTitle(title.c_str());
		fps = 0;
		secondCounter -= 1.0f;
	}

	_shader.UniformMat4f("vMat", _camera.GetMatrix());

	if (Input::GetKeyPressed(GLFW_KEY_SPACE))
	{
		if (++currentObjIndex >= (int)_objects.size())
			currentObjIndex = 0;
		d = _objects[currentObjIndex];
	}

	//////////INPUT//////////
	//ROTATION
	if (Input::GetKeyDown(GLFW_KEY_LEFT))
	{
		d->transform.Translate(glm::vec3(-moveSpeed, 0, 0));
	}
	if (Input::GetKeyDown(GLFW_KEY_RIGHT))
	{
		d->transform.Translate(glm::vec3(moveSpeed, 0, 0));
	}
	if (Input::GetKeyDown(GLFW_KEY_UP))
	{
		d->transform.Translate(glm::vec3(0, moveSpeed, 0));
	}
	if (Input::GetKeyDown(GLFW_KEY_DOWN))
	{
		d->transform.Translate(glm::vec3(0, -moveSpeed, 0));
	}

	//d->transform.SetPosition(_camera.GetCenter());

	//TRANSLATION
	if (Input::GetKeyDown(GLFW_KEY_W))
	{
		//_camera.Translate(moveSpeed * _camera.GetForward());
		_camera.Rotate(glm::vec3(5, 0, 0));
		//_camera.TranslateCenter(moveSpeed * _camera.GetForward());
	}
	if (Input::GetKeyDown(GLFW_KEY_S))
	{
		//_camera.Translate(-moveSpeed * _camera.GetForward());
		_camera.Rotate(glm::vec3(-5, 0, 0));
		//_camera.TranslateCenter(-moveSpeed * _camera.GetForward());
	}
	if (Input::GetKeyDown(GLFW_KEY_A))
	{
		//_camera.Translate(-moveSpeed * _camera.GetRight());
		_camera.Rotate(glm::vec3(0, -5, 0));
		//_camera.TranslateCenter(-moveSpeed * _camera.GetRight());
	}
	if (Input::GetKeyDown(GLFW_KEY_D))
	{
		//_camera.Translate(moveSpeed * _camera.GetRight());
		_camera.Rotate(glm::vec3(0, 5, 0));
		//_camera.TranslateCenter(moveSpeed * _camera.GetRight());
	}

	if (Input::GetKeyDown(GLFW_KEY_R))
	{
		d->transform.Translate(glm::vec3(0, 0, -moveSpeed));
	}
	if (Input::GetKeyDown(GLFW_KEY_F))
	{
		d->transform.Translate(glm::vec3(0, 0, moveSpeed));
	}

	if (Input::GetMButtonDown(GLFW_MOUSE_BUTTON_LEFT))
	{
		
		glm::vec2 mmove = Input::GetMouseMovement();
		if (glm::length(mmove) > 0)
		{
			h._controlPoints[selected] += mmove / 800.0f;
			//_camera.RotateCenterY(mmove.x * deltaTime * 10);
			//_camera.RotateCenterX(-mmove.y * deltaTime * 10);
			//_camera.Rotate(glm::vec3(-mmove.y * 0.1, mmove.x * 0.1, 0));
		}
	}
	float scroll = Input::GetScroll();
	if (scroll != 0)
		selected += scroll;
	if (selected > 3)
		selected = 0;
	else if (selected < 0)
		selected = 3;

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

static void DrawHermite(Hermite& h, Shader& s)
{
	std::vector<glm::vec2> points = h.ComputePoints(0.05f);
	VertexBuffer vb(&points[0], points.size() * sizeof(glm::vec2));
	BufferLayout layout;
	VertexArray va;
	layout.AddElement(LayoutElement(2, GL_FLOAT, ATTRIB_VERTEX_POS));
	va.AddBuffers(vb, layout);

	va.Bind();
	s.Bind();

	glLineWidth(2);
	glPointSize(5);
	glDrawArrays(GL_LINE_STRIP, 0, points.size());
	glDrawArrays(GL_POINTS, 0, points.size());
}