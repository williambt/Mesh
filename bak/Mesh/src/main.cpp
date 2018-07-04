#include "Window/Window.h"
#include "Scenes/MainScene.h"


std::ostream& operator<<(std::ostream& stream, glm::vec4 vec)
{
	stream << vec.x << ", " << vec.y << ", " << vec.z;
	return stream;
}

std::ostream& operator<<(std::ostream& stream, glm::mat4 mat)
{
	for (int i = 0; i < 4; ++i)
	{
		stream << (glm::vec4)mat[i] << std::endl;
	}
	return stream;
}

int main(int argc, char* argv[])
{

	std::cout << glm::lookAt(glm::vec3(5, 2, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	std::cin.get();

	return 0;


	Window::Create(800, 800, "Model!");
	Input::SetupCallbacks();

	MainScene scene;
	scene.Setup();

	while (Window::IsOpen())
	{
		scene.Update();
		scene.Draw();
	}

	return 0;
}