#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "control.h"

class Window {
	GLFWwindow* glfwWindow;
	Control* control;
public:
	static void Init() {
		glfwInit();
	}

	Window(const char* title, int width, int height) {
		glfwWindow = glfwCreateWindow(width, height, title, 0, 0);
		control = new Control(glfwWindow);
		glfwMakeContextCurrent(glfwWindow);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	}

	void PostUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(glfwWindow);
	}

	void PreUpdate() {
		glClearColor(0.72, 0.92, 0.98, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	Control* GetControl() {
		return control;
	}

	GLFWwindow* GetGLFWWindow() {
		return glfwWindow;
	}

	~Window() {
		glfwTerminate();
	}
};