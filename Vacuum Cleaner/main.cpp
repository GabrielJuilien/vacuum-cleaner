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
	Button* FillButton = NULL;

	GraphNode* Graph = NULL;
	GraphNode* tmp = NULL;

	GraphData* graphData = NULL;

	int i, j;

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
		GraphRectangleButton = new Button(renderer, "", "Create Graph", { 0, 0, 200, 0 }, 24, { 0, 0, 0, 0 }, { 82, 38, 0, 0 }, { 20, 20, 64, 64 }, { 0, 200, 260, 100 }, switchToolToCGraph);
		FillButton = new Button(renderer, "", "", { 128, 128, 128, 0 }, 24, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 300, 260, 420 }, switchToolToNone);
		view = new View(renderer, { 0, 0, 1020, 20, false }, { 0, 0, 20, 720, false }, { 20, 20, 1000, 700, false });
		graphData = new GraphData();
		graphData->m_drawing = view->drawing();
		graphData->m_graph = &Graph;
	}
	catch (const char* exception) {
		std::cerr << exception << std::endl;
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();
		return EXIT_FAILURE;
	}

	Graph = new GraphNode(NodeType::null, NodeState::null, NULL, NULL, NULL, NULL, 21 * PX_SIZE, 281 * PX_SIZE);
	for (i = 21 * PX_SIZE; i <= 720 * PX_SIZE; i += PX_SIZE)
	{
		for (j = 281 * PX_SIZE; j <= 1280 * PX_SIZE; j += PX_SIZE)
		{
			if (i == 21 * PX_SIZE && j == 281 * PX_SIZE) j += PX_SIZE;
			Graph->InsertNode(Graph, i, j);
		}
	}

	//Main loop
	while (currentStep != Step::QUIT) {
		handler(renderer, &currentStep, AddRectangleButton, RmvRectangleButton, GraphRectangleButton, FillButton, view, graphData);
	}

	delete AddRectangleButton;
	AddRectangleButton = NULL;
	delete RmvRectangleButton;
	RmvRectangleButton = NULL;
	delete GraphRectangleButton;
	GraphRectangleButton = NULL;
	delete FillButton;
	FillButton = NULL;

	// Destroy
<<<<<<< Updated upstream
	delete Graph;
=======
	if (Graph)
	{
		Graph->DestroyNode();
		delete Graph;
	}
>>>>>>> Stashed changes

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();

	_CrtDumpMemoryLeaks();
	system("pause");
	return EXIT_SUCCESS;
}
