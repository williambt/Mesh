#include "Window/Window.h"
#include "Scenes/MainScene.h"
#include "Scenes/DeferredShading.h"
#include "Scenes/CurveTest.h"

#include "Mesh/Writer.h"

int main(int argc, char* argv[])
{
	Window::Create(800, 800, "Model!");
	Input::SetupCallbacks();

	//MainScene scene;
	//DeferredShading scene;
	CurveTest scene;
	scene.Setup();

	while (Window::IsOpen())
	{
		scene.Draw();
		scene.Update();
	}

	return 0;
}