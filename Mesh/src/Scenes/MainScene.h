#pragma once

#include "Scene.h"
#include "../Curves/Hermite.h"

class MainScene : public Scene
{
private:

	Hermite h;
	Shader lineShader;

	float lastTime = 0;
	float secondCounter = 0;
	int fps = 0;

	Drawable* d;
	float moveSpeed = 0.15f;
	float rotateSpeed = glm::radians(5.0f);
	float camSensitivity = 1;

	bool cursorLocked = false;

	int currentObjIndex = 0;

public:

	void Setup();
	void Update();
};