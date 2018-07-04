#include "Window.h"

#include <iostream>

void Window::Create(int width, int height, const char* title)
{
	_width = width;
	_height = height;

	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW!" << std::endl;
		return;
	}

	_window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!_window)
	{
		std::cerr << "Failed to create window!" << std::endl;
		return;
	}

	glfwMakeContextCurrent(_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		return;
	}

	glEnable(GL_DEPTH_TEST);
}

Window::~Window(void)
{
	if (_window)
		glfwDestroyWindow(_window);
	glfwTerminate();
}

GLFWwindow* Window::_window;
int Window::_width;
int Window::_height;