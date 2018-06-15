#include "Window/Window.h"
#include "Scenes/MainScene.h"

int main(int argc, char* argv[])
{
	Window::Create(800, 800, "Model!");
	Input::SetupCallbacks();

	MainScene scene;
	scene.Setup();

	while (Window::IsOpen())
	{
		scene.Draw();
		scene.Update();
	}

	return 0;
}