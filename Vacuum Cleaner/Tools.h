#pragma once
#include "Button.h"
#include "Text.h"
#include "Robot.h"
#include <thread>
#include <future>

enum class Tool {
	NONE,
	DRAW,
	ERASE,
	ROBOT,
	CGRAPH
};

enum class Step {
	DRAW,
	SIMULATION_RENDERING,
	QUIT
};

struct GraphData {
	Robot** m_robot;
	SDL_Point m_startingPoint;

	GraphNode** m_graph;
	std::vector<Rect*>* m_drawing;
};

int switchToolToDraw(void* input);
int switchToolToErase(void* input);
int switchToolToNone(void* input);
int switchToolToPlaceRobot(void* input);
int switchToolToCGraph(void* input);

void handler(SDL_Renderer* p_renderer, Step* currentStep, Button* AddRectangleButton, Button* RmvRectangleButton, Button* SetRobotPosButton, Button* GraphRectangleButton, Button* FillButton, View* p_view, GraphData* p_graphData);
void drawPhaseRender(SDL_Renderer* p_renderer, Step currentStep, View* view, Button* AddRectangleButton, Button* RmvRectangleButton, Button* GraphButton, Button* SetRobotPosButton, Button* FillButton);