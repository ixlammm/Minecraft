#pragma once

#include "window.h"
#include "script.h"
#include "camera.h"
#include <vector>


class MinecraftGame {
	Camera* maincamera;
	std::vector<MinecraftScript*> scripts;
public:

	Control& control;
	Window &window;

	MinecraftGame(Window& window, Control& control): window(window), control(control) {
		maincamera = new Camera({0, 20, 0}, {0, 0, -1});
	}

	int GetKey(int key) {
		return glfwGetKey(window.GetGLFWWindow(), key) == GLFW_PRESS;
	}

	template<class T>
	void RegisterScript() {
		MinecraftScript* script = new T(*this);
		scripts.push_back(script);
	}

	void Start() {
		for (auto sc : scripts)
			sc->Start();

		while (!glfwWindowShouldClose(window.GetGLFWWindow()))
			Update();
	}

	void Update() {
		window.PreUpdate();

		for (auto sc : scripts)
			sc->Update();

		window.PostUpdate();
	}

};