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

class WorldGenerator;

class Chunk {
public:
	std::vector<float> buffer;
	static unsigned VAO, VBO;

	static void Init() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}

	void LoadChunk(int (*gbap)(glm::vec3, glm::vec3), glm::vec3 cpos) {
		buffer.clear();
		for (int i(0); i < 16; i++) {
			for (int j(0); j < 16; j++) {
				for (int k(0); k < 16; k++) {
					int c = gbap(glm::vec3(i, j, k), cpos);
					if (c != MC_BLOCK_AIR_ID) {
						glm::vec3 translate(i, j, k);
						float* faces[6]{ nullptr };
						if (j + 1 > 15 || gbap(glm::vec3(i, j + 1, k), cpos) == 0)	faces[0] = FaceUpPrefab::m_data;
						if (/*j - 1 < 00 || */gbap(glm::vec3(i, j - 1, k), cpos) == 0)	faces[1] = FaceDownPrefab::m_data;
						if (gbap(glm::vec3(i + 1, j, k), cpos) == 0)					faces[2] = FaceRightPrefab::m_data;
						if (gbap(glm::vec3(i - 1, j, k), cpos) == 0)					faces[3] = FaceLeftPrefab::m_data;
						if (gbap(glm::vec3(i, j, k + 1), cpos) == 0)					faces[4] = FaceBackPrefab::m_data;
						if (gbap(glm::vec3(i, j, k - 1), cpos) == 0)					faces[5] = FaceFrontPrefab::m_data;

						for (int f(0); f < 6; f++) {
							float* data = faces[f];
							unsigned texture_id = CUBES[c].texture_id[f];
							if (data != nullptr)
								for (int l(0); l < 6; l++) {
									glm::vec3 vp(data[l * 8 + 0], data[l * 8 + 1], data[l * 8 + 2]);
									glm::vec2 tc(data[l * 8 + 3], data[l * 8 + 4]);
									glm::vec3 n(data[l * 8 + 5], data[l * 8 + 6], data[l * 8 + 7]);
									vp += translate;
									tc = glm::vec2(
										float(texture_id % 255) / 256.f,
										float(texture_id / 255) / 256.f)
										+ tc * glm::vec2(1.f / 256.f);
									buffer.insert(buffer.end(), { vp.x, vp.y, vp.z, tc.x, tc.y, n.x, n.y, n.z });
								}
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

	static int GetBlockAtPos(glm::vec3 bpos, glm::vec3 cpos) {
		float h = (glm::perlin((glm::vec2(bpos.x, bpos.z) + glm::vec2(cpos) * glm::vec2(16)) / 10.f) + 1) * 2;
		int id = 0;
		if (bpos.y < h * 3 + 3) {
			if (GetBlockAtPos(bpos + glm::vec3(0, 1, 0), cpos) == MC_BLOCK_AIR_ID)
				id = MC_BLOCK_GRASS_PATH_ID;
			else
				id = MC_BLOCK_DIRT_ID;
		}
		if (bpos.y < h * 3) id = 2;
		return id;
	}

public:

	WorldGenerator(float seed, unsigned distance, glm::vec3 eyepos): seed(seed), distance(distance) {
		Chunk::Init();
		field = new Chunk*[distance * distance];
		position = glm::vec3(eyepos.x, eyepos.z, eyepos.y) / glm::vec3(16);
		for (int i(0); i < distance; i++) {
			for (int j(0); j < distance; j++) {
				field[j * distance + i] = nullptr;
			}
		}
	}

	void ShiftField(glm::ivec2 sv) {

	}

	void Update(glm::vec3 cpos) {
		glm::ivec3 _cpos = glm::vec3(cpos.x, cpos.z, cpos.y) / glm::vec3(16);
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
		if (_cpos.y > position.y) {
			for (int i(0); i < distance; i++) {
				if (field[i] != nullptr) delete field[i];
				for (int j(0); j < distance - 1; j++) {
					field[j * distance + i] = field[(j + 1) * distance + i];
				}
				field[(distance - 1) * distance + i] = nullptr;
			}
		}
		else if (_cpos.y < position.y) {
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
					field[j * distance + i] = new Chunk();
					field[j * distance + i]->LoadChunk(GetBlockAtPos, glm::vec3(_cpos) + glm::vec3(i - int(distance / 2), j - int(distance / 2), 0));
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