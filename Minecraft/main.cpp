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

#define WIDTH 800
#define HEIGHT 550

Camera camera(glm::vec3(0, 0, 2), glm::vec3(0, 0, -1));

#define KEYS 0.05f
#define CAMS 0.006f

//Texture GenNoise(int x, int y, int seed = 0x12345) {
//	unsigned char* data = new unsigned char[x * y * 3];
//	for (int j(0); j < y; j++) {
//		for (int i(0); i < x * 3; i+=3) {
//
//			int v = int((glm::perlin(glm::vec4(float(i) / 300.f, float(j) / 100.f, seed)) + 1) * 0.5 * 255.f);
//			data[j * x * 3 + i] = v;
//			data[j * x * 3 + i + 1] = v;
//			data[j * x * 3 + i + 2] = v;
//		}
//	}
//	Texture t;
//	t.LoadFromData(data, x, y);
//	return t;
//}

unsigned chunk[16][16][16];

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

	for (int i(0); i < 16; i++) {
		for (int j(0); j < 16; j++) {
			for (int k(0); k < 16; k++) {
				chunk[i][j][k] = rand() % 2;
			}
		}
	}

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

	InitFacePrefab<FaceUpPrefab>();
	InitFacePrefab<FaceDownPrefab>();
	InitFacePrefab<FaceRightPrefab>();
	InitFacePrefab<FaceLeftPrefab>();
	InitFacePrefab<FaceFrontPrefab>();
	InitFacePrefab<FaceBackPrefab>();

	//Texture noise = GenNoise(1024, 1024);

	rm.LoadResources();

	glEnable(GL_DEPTH_TEST);

	Shader shader("vs.vert", "fs.frag");
	shader.Use();

	glViewport(0, 0, WIDTH, HEIGHT);

	glm::mat4 projection = glm::perspective(45.0, double(WIDTH) / HEIGHT, 0.1, 100.0);

	glUniformMatrix4fv(glGetUniformLocation(shader.GetPID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glfwWindowHint(GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);

	while (!glfwWindowShouldClose(window)) {

		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double xpos, ypos;

		glfwGetCursorPos(window, &xpos, &ypos);

		glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);
		double deltaX = WIDTH / 2 - xpos, deltaY = HEIGHT / 2 - ypos;

		glm::mat4 transform(1);

		camera.Rotate(glm::vec3(deltaY * CAMS, deltaX * CAMS, 0));

		glActiveTexture(GL_TEXTURE1);

		shader.SetMat4("transform", transform);
		shader.SetMat4("view", camera.GetView());
		shader.SetUint("text", 1);



		for (int i(0); i < 16; i++) {
			for (int j(0); j < 16; j++) {
				for (int k(0); k < 16; k++) {
					
					if (chunk[i][j][k] != MC_BLOCK_AIR_ID) {

						shader.SetMat4("transform", glm::translate(glm::vec3(i, j, k)));

						if (j + 1 > 15 || chunk[i][j + 1][k] == 0) {
							glBindVertexArray(FaceUpPrefab::VAO);
							glDrawArrays(GL_TRIANGLES, 0, 6);

						}
						if (j - 1 < 0 || chunk[i][j - 1][k] == 0) {
							glBindVertexArray(FaceDownPrefab::VAO);
							glDrawArrays(GL_TRIANGLES, 0, 6);
						}
						if (i + 1 > 15 || chunk[i + 1][j][k] == 0) {
							glBindVertexArray(FaceRightPrefab::VAO);
							glDrawArrays(GL_TRIANGLES, 0, 6);
						}
						if (i - 1 < 0 || chunk[i - 1][j][k] == 0) {
							glBindVertexArray(FaceLeftPrefab::VAO);
							glDrawArrays(GL_TRIANGLES, 0, 6);
						}
						if (k + 1 > 15 || chunk[i][j][k + 1] == 0) {
							glBindVertexArray(FaceBackPrefab::VAO);
							glDrawArrays(GL_TRIANGLES, 0, 6);
						}
						if (k - 1 < 0 || chunk[i][j][k - 1] == 0) {
							glBindVertexArray(FaceFrontPrefab::VAO);
							glDrawArrays(GL_TRIANGLES, 0, 6);
						}
					}
				}
			}
		}


		glfwPollEvents();
		glfwSwapBuffers(window);
		KeyUpdate(window);
	}

	glfwTerminate();
}