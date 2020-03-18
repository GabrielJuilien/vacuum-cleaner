#pragma once
#include "Button.h"
#include "Text.h"
#include "View.h"

enum class Tool {
	NONE,
	DRAW,
	ERASE,
	CGRAPH
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
int switchToolToCGraph(void* input);

void handler(SDL_Renderer* p_renderer, Step* currentStep, Button* AddRectangleButton, Button* RmvRectangleButton, Button* GraphRectangleButton, Button* FillButton);
void render(SDL_Renderer* p_renderer, Step currentStep, View* view, Button* AddRectangleButton, Button* RmvRectangleButton, Button* GraphRectangleButton, Button* FillButton);