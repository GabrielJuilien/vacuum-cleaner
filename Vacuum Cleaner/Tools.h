#pragma once
#include "utils.h"
#include "Text.h"

enum class Tool {
	NONE,
	DRAW,
	ERASE
};

enum class Step {
	HOME,
	DRAW,
	SIMULATION_LOADING,
	SIMULATION_RENDERING,
	QUIT
};

int switchToolToDraw(void* input);
int switchToolToErase(void* input);
int switchToolToNone(void* input);

void handler(SDL_Renderer* p_renderer, Step currentStep, Button* AddRectangleButton, Button* RmvRectangleButton, Button* FillButton)
void render
