#include "shader.h"
#include <GLFW/glfw3.h>

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/noise.hpp>

#include "camera.h"
#include "faces.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "game.h"

#include <random>

#include "script.h"
#include "cubes.h"
#include "rsrcmng.h"
#include "chunk.h"

#define WIDTH 1920
#define HEIGHT 1080
#define DISTANCE 10
#define KEYSS 3.f
#define CAMS 0.006f


Camera camera(glm::vec3(0, 20, 0), glm::vec3(0, 0, -1));

class MainScript : public MinecraftScript {
	using MinecraftScript::MinecraftScript;

	Shader* shader;
	WorldGenerator *wg;

	void Start() {
		shader = new Shader("vs.vert", "fs.frag");

		wg = new WorldGenerator(4.f, DISTANCE, glm::vec3(0));

		shader->Use();
		shader->SetVec3("lightDir", glm::vec3(-0.5, -1, -1));
		shader->SetVec3("ambient", glm::vec3(0.5));	

		glm::mat4 projection = glm::perspective(45.0, double(WIDTH) / HEIGHT, 0.01, 300.0);
		glUniformMatrix4fv(glGetUniformLocation(shader->GetPID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	}

	void Update() {
		glm::ivec2 pos = game.control.GetMousePosition();

		game.control.SetMousePosition({ WIDTH / 2, HEIGHT / 2 });
		double deltaX = WIDTH / 2 - pos.x, deltaY = HEIGHT / 2 - pos.y;

		glm::mat4 transform(1);

		camera.Rotate(glm::vec3(deltaY * CAMS, deltaX * CAMS, 0));

		shader->SetVec3("camPos", camera.GetPosition());
		shader->SetMat4("transform", transform);
		shader->SetMat4("view", camera.GetView());
		shader->SetUint("text", 0);

		wg->Update(camera.GetPosition() * glm::vec3(1, 0, 1));
		int cif = 0;
		for (int i(0); i < DISTANCE; i++) {
			for (int j(0); j < DISTANCE; j++) {
				glm::vec3 pos = (glm::vec3(i - (DISTANCE) / 2, 0, j - (DISTANCE) / 2) + glm::vec3(glm::ivec3(camera.GetPosition() / glm::vec3(16)))) * glm::vec3(16, 0, 16);
				if (glm::dot(glm::normalize(pos + camera.Front() * glm::vec3(16, 0, 16) - camera.GetPosition()), camera.Front())> 0) {
					transform = glm::translate(pos);
					wg->GetField()[j * DISTANCE + i]->Use();
					shader->SetMat4("transform", transform);
					glBindVertexArray(Chunk::VAO);
					glDrawArrays(GL_TRIANGLES, 0, wg->GetField()[j * DISTANCE + i]->buffer.size() / 8);
					cif++;
				}
			}
		}

		std::cout << "Chunks in frame: " << cif << " of : "  << DISTANCE * DISTANCE << "\n";

		float KEYS = KEYSS * game.deltaTime;

		if (game.control.GetKey(GLFW_KEY_A))
			camera.Translate(-camera.Right() * KEYS);
		if (game.control.GetKey(GLFW_KEY_D))
			camera.Translate(camera.Right() * KEYS);
		if (game.control.GetKey(GLFW_KEY_W))
			camera.Translate(camera.Front() * KEYS);
		if (game.control.GetKey(GLFW_KEY_S))
			camera.Translate(-camera.Front() * KEYS);
		if (game.control.GetKey(GLFW_KEY_SPACE))
			camera.Translate(WORLD_UP * KEYS);
		if (game.control.GetKey(GLFW_KEY_C))
			camera.Translate(-WORLD_UP * KEYS);
		if (game.control.GetKey(GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(game.window.GetGLFWWindow(), true);
		if (game.control.GetKey(GLFW_KEY_F11)) {
			glfwSetWindowMonitor(game.window.GetGLFWWindow(), glfwGetPrimaryMonitor(), 0, 0, 1920, 1080, GLFW_DONT_CARE);
		}
	}
};

ResourceManager rm;

int main() {

	Window::Init();
	Window win("Minecraft", WIDTH, HEIGHT);

	MinecraftGame game(win, *win.GetControl());
	game.RegisterScript<MainScript>();

	rm.LoadResources();

	glEnable(GL_DEPTH_TEST);

	glfwSwapInterval(0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_CW);

	glViewport(0, 0, WIDTH, HEIGHT);
	glfwWindowHint(GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetInputMode(win.GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetCursorPos(win.GetGLFWWindow(), WIDTH / 2, HEIGHT / 2);

	game.Start();
}