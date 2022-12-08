#pragma once

#include <vector>
#include "texture.h"
#include "textures.h"

unsigned Texture::txs_count = 0;

class ResourceManager {
	Texture CubesTexture;

	void LoadCubesTexture() {
		int width = 4096;
		int height = 4096;
		unsigned char* cubes_texture_data = new unsigned char[width * height * 3];
		for (int k(0); k < MC_TEXTURES_COUNT; k++) {
			int w, h;
			int p = (k / 256) * 16 * 4096 * 3;
			unsigned char* data = stbi_load(MC_TEXTURE_ID_FN_MAP[k], &w, &h, 0, 3);
			for (int j(0); j < 16; j++)
				for (int i(0); i < 16 * 3; i++)
					cubes_texture_data[(k % 256) * 16 * 3 + p + j * 4096 * 3 + i] = data[j * w * 3 + i];
		}
		CubesTexture.LoadFromData(cubes_texture_data, width, height);
		delete[] cubes_texture_data;
	}
public:
	ResourceManager() {
	}

	void LoadResources() {
		LoadCubesTexture();
	}

	Texture& GetCubesTexture() {
		return CubesTexture;
	}
};