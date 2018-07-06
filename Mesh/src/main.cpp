#include "Window/Window.h"
#include "Scenes/MainScene.h"
#include "Scenes/DeferredShading.h"
#include "Scenes/CurveTest.h"
#include "Scenes/RaceTrackScene.h"

#include "Mesh/Writer.h"

int main(int argc, char* argv[])
{
	Window::Create(800, 800, "Model!");
	Input::SetupCallbacks();

	//MainScene scene;
	//DeferredShading scene;
	//CurveTest scene; //Cria��o da pista
	RaceTrackScene scene; //Visualiza��o da pista
	scene.Setup();

	while (Window::IsOpen())
	{
		scene.Draw();
		scene.Update();
	}

	return 0;
}