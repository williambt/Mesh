#include "Window/Window.h"
#include "Scenes/MainScene.h"
#include "Scenes/DeferredShading.h"

int main(int argc, char* argv[])
{
	Window::Create(800, 800, "Model!");
	Input::SetupCallbacks();

	//MainScene scene;
	DeferredShading scene;
	scene.Setup();

	while (Window::IsOpen())
	{
		scene.Draw();
		scene.Update();
	}

	return 0;
}