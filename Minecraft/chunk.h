#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>
#include <glad/glad.h>
#include <vector>
#include "cubes.h"
#include "faces.h"
#include "textures.h"

/*
	- CHUNK DATA IN MEMORY -
	VERTICES (VEC3), TEXTURE_COORDINATES (VEC2)
*/

class Chunk {
public:
	std::vector<float> buffer;
	static unsigned VAO, VBO;

	static void Init() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	void LoadData(unsigned cubes[16][16][16]) {
		buffer.clear();
		for (int i(0); i < 16; i++) {
			for (int j(0); j < 16; j++) {
				for (int k(0); k < 16; k++) {
					if (cubes[i][j][k] != MC_BLOCK_AIR_ID) {
						glm::vec3 translate(i, j, k);
						std::vector<float*> faces;
						if (j + 1 > 15 || cubes[i][j + 1][k] == 0) faces.push_back(FaceUpPrefab		::m_data);
						if (j - 1 < 00 || cubes[i][j - 1][k] == 0) faces.push_back(FaceDownPrefab	::m_data);
						if (i + 1 > 15 || cubes[i + 1][j][k] == 0) faces.push_back(FaceRightPrefab	::m_data);
						if (i - 1 < 00 || cubes[i - 1][j][k] == 0) faces.push_back(FaceLeftPrefab	::m_data);
						if (k + 1 > 15 || cubes[i][j][k + 1] == 0) faces.push_back(FaceBackPrefab	::m_data);
						if (k - 1 < 00 || cubes[i][j][k - 1] == 0) faces.push_back(FaceFrontPrefab	::m_data);

						for (auto data: faces)
						for (int l(0); l < 6; l++) {
							glm::vec3 vp(data[l * 5 + 0], data[l * 5 + 1], data[l * 5 + 2]);
							glm::vec2 tc(data[l * 5 + 3], data[l * 5 + 4]);
							vp += translate;
							tc = glm::vec2(
								float((cubes[i][j][k] - 1) % 256) / 256.f,
								float((cubes[i][j][k] - 1) / 256) / 256.f)
								+ tc * glm::vec2(1.f / 256.f);
							buffer.insert(buffer.end(), { vp.x, vp.y, vp.z, tc.x, tc.y });
						}
					}
				}
			}
		}
	}

	void Use() {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), buffer.data(), GL_DYNAMIC_DRAW);
		glBindVertexArray(0);
	}
};

class WorldGenerator {
	float seed;
	unsigned distance;
	unsigned chunk[16][16][16];
	Chunk** field;
	glm::ivec3 position;
public:

	WorldGenerator(float seed, unsigned distance, glm::vec3 eyepos): seed(seed), distance(distance) {
		Chunk::Init();
		field = new Chunk*[distance * distance];
		position = eyepos / glm::vec3(16);
		for (int i(0); i < distance; i++) {
			for (int j(0); j < distance; j++) {
				field[j * distance + i] = nullptr;
			}
		}
	}

	void ShiftField(glm::ivec2 sv) {

	}

	void Update(glm::vec3 cpos) {
		glm::ivec3 _cpos = cpos / glm::vec3(16);
		if (_cpos.x > position.x) {
			for (int j(0); j < distance; j++) {
				if (field[j * distance + 0] != nullptr) delete field[j * distance + 0];
				for (int i(0); i < distance - 1; i++) {
					field[j * distance + i] = field[j * distance + i + 1];
				}
				field[j * distance + distance - 1] = nullptr;
			}
		}
		else if (_cpos.x < position.x) {
			for (int j(0); j < distance; j++) {
				if (field[j * distance + distance - 1] != nullptr) delete field[j * distance + distance - 1];
				for (int i(distance - 1); i > 0; i--) {
					field[j * distance + i] = field[j * distance + i - 1];
				}
				field[j * distance + 0] = nullptr;
			}
		}
		if (_cpos.z > position.z) {
			for (int i(0); i < distance; i++) {
				if (field[i] != nullptr) delete field[i];
				for (int j(0); j < distance - 1; j++) {
					field[j * distance + i] = field[(j + 1) * distance + i];
				}
				field[(distance - 1) * distance + i] = nullptr;
			}
		}
		else if (_cpos.z < position.z) {
			for (int i(0); i < distance; i++) {
				if (field[(distance - 1) * distance + i] != nullptr) delete field[(distance - 1) * distance + i];
				for (int j(distance - 1); j > 0; j--) {
					field[j * distance + i] = field[(j - 1) * distance + i];
				}
				field[i] = nullptr;
			}
		}
		for (int i(0); i < distance; i++) {
			for (int j(0); j < distance; j++) {
				if (field[j * distance + i] == nullptr) {
					for (int x(0); x < 16; x++) {
						for (int y(0); y < 16; y++) {
							float h = (glm::perlin((glm::vec3(x, y, 0) + (glm::vec3(_cpos) + glm::vec3(i - 1, j - 1, 0)) * glm::vec3(16)) / 10.f) + 1) * 2;
							for (int z(0); z < 16; z++) {
								chunk[x][z][y] = z < h * 3;
							}
						}
					}
					field[j * distance + i] = new Chunk();
					field[j * distance + i]->LoadData(chunk);
				}
			}
		}
		position = _cpos;
	}

	Chunk** GetField() {
		return field;
	}
};

unsigned Chunk::VAO = 0;
unsigned Chunk::VBO = 0;