#pragma once

#include "control.h"

class MinecraftGame;

class MinecraftScript {
protected:
	MinecraftGame& game;
public:
	MinecraftScript(MinecraftGame& game): game(game) {}

	virtual void Start() = 0;

	virtual void Update() = 0;

	friend MinecraftGame;
};