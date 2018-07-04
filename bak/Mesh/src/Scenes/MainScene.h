#pragma once

#include "Scene.h"

class MainScene : public Scene
{
private:

	float lastTime = 0;
	float secondCounter = 0;
	int fps = 0;

	Drawable* d;
	float moveSpeed = 0.15f;
	float rotateSpeed = glm::radians(5.0f);

	int currentObjIndex = 0;

public:

	void Setup();
	void Update();
};