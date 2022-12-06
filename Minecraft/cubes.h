#pragma once

#include "textures.h"

#define MAX_CUBES 2

#define MC_BLOCK_AIR_ID 0
#define MC_BLOCK_DIRT_ID 1

typedef struct {

	unsigned block_id;
	unsigned texture_id;

} CubeDescription;

CubeDescription CUBES[MAX_CUBES] = {
	{ MC_BLOCK_AIR_ID, NULL },
	{ MC_BLOCK_DIRT_ID, MC_TEXTURE_DIRT_ID }
};