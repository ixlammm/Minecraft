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

#include <random>

#include "cubes.h"
#include "rsrcmng.h"
#include "chunk.h"

#define WIDTH 800
#define HEIGHT 550
#define DISTANCE 6
#define KEYS 0.05f
#define CAMS 0.006f


Camera camera(glm::vec3(0, 20, 0), glm::vec3(0, 0, -1));


void KeyUpdate(GLFWwindow* window)
{
	if (glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS)
		camera.Translate(-camera.Right() * KEYS);
	if (glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS)
		camera.Translate(camera.Right() * KEYS);
	if (glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS)
		camera.Translate(camera.Front() * KEYS);
	if (glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS)
		camera.Translate(-camera.Front() * KEYS);
	if (glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.Translate(WORLD_UP * KEYS);
	if (glfwGetKey(window,GLFW_KEY_C) == GLFW_PRESS)
		camera.Translate(-WORLD_UP * KEYS);
	if (glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

ResourceManager rm;

int main() {

	if (!glfwInit()) {
		std::cout << "Error: Init GLFW\n";
	}

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GLFW_ WINDOW", 0, 0);

	if (!window) {
		std::cout << "Error: glfw window";
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader(GLADloadproc(glfwGetProcAddress));

	//Texture noise = GenNoise(1024, 1024);

	rm.LoadResources();

	glEnable(GL_DEPTH_TEST);

	Shader shader("vs.vert", "fs.frag");
	shader.Use();

	WorldGenerator wg(4.f, DISTANCE, glm::vec3(0));

	glViewport(0, 0, WIDTH, HEIGHT);

	glm::mat4 projection = glm::perspective(45.0, double(WIDTH) / HEIGHT, 0.01, 100.0);

	glUniformMatrix4fv(glGetUniformLocation(shader.GetPID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glfwWindowHint(GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);

	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.80, 0.98, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double xpos, ypos;

		glfwGetCursorPos(window, &xpos, &ypos);

		glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);
		double deltaX = WIDTH / 2 - xpos, deltaY = HEIGHT / 2 - ypos;

		glm::mat4 transform(1);

		camera.Rotate(glm::vec3(deltaY * CAMS, deltaX * CAMS, 0));

		shader.SetMat4("transform", transform);
		shader.SetMat4("view", camera.GetView());
		shader.SetUint("text", 1);

		wg.Update(camera.GetPosition());
		for (int i(0); i < DISTANCE; i++) {
			for (int j(0); j < DISTANCE; j++) {
				transform = glm::translate((glm::vec3(i - (DISTANCE) / 2, 0, j - (DISTANCE) / 2) + glm::vec3(glm::ivec3(camera.GetPosition() / glm::vec3(16)))) * glm::vec3(16, 0, 16));
				wg.GetField()[j * DISTANCE + i]->Use();
				shader.SetMat4("transform", transform);
				glBindVertexArray(Chunk::VAO);
				glDrawArrays(GL_TRIANGLES, 0, wg.GetField()[j * DISTANCE + i]->buffer.size() / 5);
			}
		}

		glfwPollEvents();
		glfwSwapBuffers(window);
		KeyUpdate(window);
	}

	glfwTerminate();
}