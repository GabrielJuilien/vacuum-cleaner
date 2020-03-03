#pragma once
#include "utils.h"
#include "Text.h"

enum class Tool {
	NONE,
	DRAW,
	ERASE
};

int switchToolToDraw(void* input);
int switchToolToErase(void* input);
int switchToolToNone(void* input);
