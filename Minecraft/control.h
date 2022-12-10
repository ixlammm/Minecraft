#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Control {
	GLFWwindow* window;
public:
	Control(GLFWwindow* window): window(window) {}

	glm::ivec2 GetMousePosition() {
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return { x, y };
	}

	void SetMousePosition(glm::ivec2 pos) {
		glfwSetCursorPos(window, pos.x, pos.y);
	}

	int GetKey(int key) {
		return glfwGetKey(window, key) == GLFW_PRESS;
	}
};