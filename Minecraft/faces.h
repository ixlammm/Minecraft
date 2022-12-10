#pragma once

#include "prefab.h"

#define FACE_DATA_SIZE 48

#define DEFINE_FACE(name) class name { \
			public:\
			static float m_data[FACE_DATA_SIZE]; \
			static unsigned VAO;\
			static unsigned VBO;\
}; INIT_FACE(name);

#define INIT_FACE(name) unsigned name::VAO; unsigned name::VBO

DEFINE_FACE(FaceUpPrefab);
DEFINE_FACE(FaceDownPrefab);
DEFINE_FACE(FaceRightPrefab);
DEFINE_FACE(FaceLeftPrefab);
DEFINE_FACE(FaceFrontPrefab);
DEFINE_FACE(FaceBackPrefab);

float FaceUpPrefab::m_data[FACE_DATA_SIZE] = {
	1, 1, 0,	1, 0,	0, 1, 0,
	0, 1, 0,	0, 0,	0, 1, 0,
	0, 1, 1,	0, 1,	0, 1, 0,

	1, 1, 0,	1, 0,	0, 1, 0,
	0, 1, 1,	0, 1,	0, 1, 0,
	1, 1, 1,	1, 1,	0, 1, 0,
};

float FaceDownPrefab::m_data[FACE_DATA_SIZE] = {
	1, 0, 1,	0, 0,	0, -1, 0,
	0, 0, 1,	1, 0,	0, -1, 0,
	0, 0, 0,	1, 1,	0, -1, 0,

	1, 0, 1,	0, 0,	0, -1, 0,
	0, 0, 0,	1, 1,	0, -1, 0,
	1, 0, 0,	0, 1,	0, -1, 0,
};

float FaceRightPrefab::m_data[FACE_DATA_SIZE] = {
	1, 0, 1,	0, 0,	1, 0, 0,
	1, 0, 0,	1, 0,	1, 0, 0,
	1, 1, 0,	1, 1,	1, 0, 0,

	1, 0, 1,	0, 0,	1, 0, 0,
	1, 1, 0,	1, 1,	1, 0, 0,
	1, 1, 1,	0, 1,	1, 0, 0,
};


float FaceLeftPrefab::m_data[FACE_DATA_SIZE] = {
	0, 0, 0,	0, 0,	-1, 0, 0,
	0, 0, 1,	1, 0,	-1, 0, 0,
	0, 1, 1,	1, 1,	-1, 0, 0,

	0, 0, 0,	0, 0,	-1, 0, 0,
	0, 1, 1,	1, 1,	-1, 0, 0,
	0, 1, 0,	0, 1,	-1, 0, 0,
};

float FaceFrontPrefab::m_data[FACE_DATA_SIZE] = {
	1, 0, 0,	0, 0,	0, 0, 1,
	0, 0, 0,	1, 0,	0, 0, 1,
	0, 1, 0,	1, 1,	0, 0, 1,

	1, 0, 0,	0, 0,	0, 0, 1,
	0, 1, 0,	1, 1,	0, 0, 1,
	1, 1, 0,	0, 1,	0, 0, 1,
};

float FaceBackPrefab::m_data[FACE_DATA_SIZE] = {
	0, 0, 1,	0, 0,	0, 0, -1,
	1, 0, 1,	1, 0,	0, 0, -1,
	1, 1, 1,	1, 1,	0, 0, -1,

	0, 0, 1,	0, 0,	0, 0, -1,
	1, 1, 1,	1, 1,	0, 0, -1,
	0, 1, 1,	0, 1,	0, 0, -1,
};


template<class T>
static void InitFacePrefab() {

	glGenVertexArrays(1, &T::VAO);
	glGenBuffers(1, &T::VBO);

	glBindVertexArray(T::VAO);
	glBindBuffer(GL_ARRAY_BUFFER, T::VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBufferData(GL_ARRAY_BUFFER, sizeof(T::m_data), T::m_data, GL_STATIC_DRAW);

	glBindVertexArray(0);
}