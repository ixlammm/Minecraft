#pragma once

#include "textures.h"

#define MAX_CUBES 4

#define MC_BLOCK_AIR_ID 0
#define MC_BLOCK_DIRT_ID 1
#define MC_BLOCK_SAND_ID 2
#define MC_BLOCK_GRAVEL_ID 3

typedef struct {

	unsigned block_id;
	unsigned texture_id;

} CubeDescription;

CubeDescription CUBES[MAX_CUBES] = {
	{ MC_BLOCK_AIR_ID, NULL },
	{ MC_BLOCK_DIRT_ID, MC_TEXTURE_DIRT_ID },
	{ MC_BLOCK_SAND_ID, MC_TEXTURE_SAND_ID },
	{ MC_BLOCK_GRAVEL_ID, MC_TEXTURE_GRAVEL_ID },
};