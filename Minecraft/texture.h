#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>

class ResourceManager;

class Texture {
	static unsigned txs_count;
	const char* name;
	unsigned gl_id;
	int id;
	int width, height;
	unsigned char* data;
public:
	Texture(): name(0), gl_id(0), id(-1), width(-1), height(-1), data(nullptr) {}

	void LoadFromFile(const char* name) {
		this->name = name;
		data = stbi_load(name, &width, &height, 0, 3);
		LoadFromData(data, width, height);
	}

	void LoadFromData(unsigned char* data, int width, int height) {
		id = txs_count++;
		this->width = width;
		this->height = height;

		glActiveTexture(GL_TEXTURE0 + id);
		glGenTextures(1, &gl_id);
		glBindTexture(GL_TEXTURE_2D, gl_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	void FreeData() {
		free(data);
	}

	unsigned GetId() {
		return id;
	}

	int GetWidth() { return width; }
	int GetHeight() { return height; }

	friend ResourceManager;
};