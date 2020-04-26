#define _CRTDBG_MAP_ALLOC
#include "utils.h"
#include "Button.h"
#include "Tools.h"
#include "Rect.h"
#include "View.h"
#include "GraphNode.h"

int main(int argc, char** argv)
{
	SDL_Window* window;
	SDL_Renderer* renderer;

	static Step currentStep = Step::DRAW;

	View* view = NULL;
	Button* AddRectangleButton = NULL;
	Button* RmvRectangleButton = NULL;
	Button* GraphRectangleButton = NULL;
	Button* SetRobotPosButton = NULL;
	Button* FillButton = NULL;

	GraphNode* Graph = NULL;
	Robot* robot = NULL;

	GraphData* graphData = NULL;

	int i = 0, j;

	int lastFrame = SDL_GetTicks(), currentFrame = SDL_GetTicks();

	if (!SDL_WasInit(SDL_INIT_VIDEO))
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			std::cerr << "[-] ERROR - Failed to initialise SDL (" << SDL_GetError() << ")\n";
			return EXIT_FAILURE;
		}
	}

	if (!TTF_WasInit()) {
		if (TTF_Init() < 0) {
			std::cerr << "[-] ERROR - Failed to initialise TTF (" << TTF_GetError() << ")\n";
			SDL_Quit();
			return EXIT_FAILURE;
		}
	}

	window = SDL_CreateWindow("Vacuum Cleaner Planner", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);

	if (!window)
	{
		std::cerr << "[-] ERROR - Failed to create SDL window (" << SDL_GetError() << ")\n";
		TTF_Quit();
		SDL_Quit();
		return EXIT_FAILURE;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	try {
		AddRectangleButton = new Button(renderer, "ressources/draw_rectangle.png", "Draw rectangle", { 0, 150, 0, 0 }, 24, { 0, 0, 0, 0 }, { 82, 38, 0, 0 }, { 20, 20, 64, 64 }, { 0, 0, 260, 100 }, switchToolToDraw);
		RmvRectangleButton = new Button(renderer, "ressources/erase_rectangle.png", "Erase rectangle", { 200, 0, 0, 0 }, 24, { 0, 0, 0, 0 }, { 82, 38, 0, 0 }, { 20, 20, 64, 64 }, { 0, 100, 260, 100 }, switchToolToErase);
		SetRobotPosButton = new Button(renderer, "ressources/robot_icon.png", "Place robot", { 0, 200, 200, 0 }, 24, { 0, 0, 0, 0 }, { 82, 38, 0, 0 }, { 20, 20, 64, 64 }, { 0, 200, 260, 100 }, switchToolToPlaceRobot);
		GraphRectangleButton = new Button(renderer, "ressources/create_graph.png", "Start simulation", { 0, 0, 200, 0 }, 24, { 0, 0, 0, 0 }, { 82, 38, 0, 0 }, { 20, 20, 64, 64 }, { 0, 300, 260, 100 }, switchToolToCGraph);

		FillButton = new Button(renderer, "", "", { 128, 128, 128, 0 }, 24, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 400, 260, 420 }, switchToolToNone);
		
		view = new View(renderer, { 0, 0, 1020, 20, false }, { 0, 0, 20, 720, false }, { 20, 20, 1000, 700, false });
		view->x(260);
		view->y(0);

		graphData = new GraphData();
		graphData->m_drawing = view->drawing();
		graphData->m_graph = &Graph;
		graphData->m_robot = &robot;
		graphData->m_startingPoint.x = 0;
		graphData->m_startingPoint.y = 0;
	}
	catch (const char* exception) {
		std::cerr << exception << std::endl;
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();
		return EXIT_FAILURE;
	}

	//Main loop
	while (currentStep == Step::DRAW) {
		handler(renderer, &currentStep, AddRectangleButton, RmvRectangleButton, GraphRectangleButton, SetRobotPosButton, FillButton, view, graphData);
	}

	while (currentStep == Step::SIMULATION_RENDERING) {
		i++;
		simulation(renderer, &currentStep, graphData);
	}


	std::thread thread1(deleteRobot, robot);

	delete AddRectangleButton;
	delete RmvRectangleButton;
	delete GraphRectangleButton;
	delete SetRobotPosButton;
	delete FillButton;
	delete view;


	if (Graph)
	{
		for (i = 699; i >= 0; i--) {
			for (j = 999; j >= 0; j--) {
				delete Graph->seekGraph(j, i);
			}
		}
	}

	thread1.join();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();

	return EXIT_SUCCESS;
}
