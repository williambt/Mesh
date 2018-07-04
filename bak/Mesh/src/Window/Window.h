#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
private:
	static GLFWwindow* _window;
	static int _width, _height;

public:
	~Window(void);
	
	static void Create(int width, int height, const char* title);

	static inline int GetWidth() { return _width; }
	static inline int GetHeight() { return _height; }
	static inline GLFWwindow* GetGLFWwindow() { return _window; }

	static inline bool IsOpen() { return !glfwWindowShouldClose(_window); }
	static inline void Close() { glfwSetWindowShouldClose(_window, GLFW_TRUE); }

	static inline void SwapBuffers() { glfwSwapBuffers(_window); }
};