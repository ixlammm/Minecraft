#pragma once

#include <vector>
#include "texture.h"
#include "textures.h"

unsigned Texture::txs_count = 0;

class ResourceManager {
	
	std::vector<Texture> textures;
public:
	ResourceManager() {}

	void LoadResources() {
		for (int i(0); i < MC_TEXTURES_COUNT; i++) {
			Texture t = Texture();
			t.LoadFromFile(MC_TEXTURE_ID_FN_MAP[i]);
			textures.push_back(t);
		}
	}

	Texture& GetTexture(unsigned texture_id) {
		return textures[texture_id];
	}
};