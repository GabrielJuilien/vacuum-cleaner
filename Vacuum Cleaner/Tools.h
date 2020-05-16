#pragma once
#include "Button.h"
#include "Text.h"
#include "Robot.h"
#include "utils.h"
#include <thread>
#include <future>
#include <iostream>
#include <string>

enum class Tool {
	NONE,
	DRAW,
	ERASE,
	ROBOT,
	CGRAPH
};

enum class Step {
	DRAW,
	DRAWING_PROCESSING,
	SIMULATION_RENDERING,
	SIMULATION_END,
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
void drawPhaseRender(SDL_Renderer* p_renderer, Step* currentStep, View* view, Button* AddRectangleButton, Button* RmvRectangleButton, Button* GraphButton, Button* SetRobotPosButton, Button* FillButton);

void simulationPhaseRender(SDL_Renderer* p_renderer, Step* p_currentStep, GraphData* p_graphData);
void simulation(SDL_Renderer* p_renderer, Step* p_currentStep, GraphData* p_graphData);

void post_simulation(SDL_Renderer* p_renderer, Step* p_currentStep);
void post_simulation_render(SDL_Renderer* p_renderer, Button* exportGraph, Button* quitProgram, std::string userPath, std::string error);

int export_graph(void* input);
int quit_program(void* input);

void deleteGraph(GraphNode* p_graph);
void deleteRobot(Robot* p_robot);
