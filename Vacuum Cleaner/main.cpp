

#define _CRTDBG_MAP_ALLOC
#include "utils.h"
#include "Button.h"
#include "Tools.h"
#include "Rect.h"


int main(int argc, char** argv)
{
	SDL_Window* window;
	SDL_Renderer* renderer;

	static Step currentStep = Step::DRAW;

	Button* AddRectangleButton = NULL;
	Button* RmvRectangleButton = NULL;
	Button* GraphRectangleButton = NULL;
	Button* FillButton = NULL;

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
		AddRectangleButton = new Button(renderer, "ressources/draw_rectangle.png", "Draw rectangle", { 0, 150, 0, 0 }, 24, { 0, 0, 0, 0 }, { 104, 38, 0, 0 }, { 20, 20, 64, 64 }, { 0, 0, 280, 100 }, switchToolToDraw);
		RmvRectangleButton = new Button(renderer, "ressources/erase_rectangle.png", "Erase rectangle", { 200, 0, 0, 0 }, 24, { 0, 0, 0, 0 }, { 104, 38, 0, 0 }, { 20, 20, 64, 64 }, { 0, 100, 280, 100 }, switchToolToErase);
		GraphRectangleButton = new Button(renderer, "", "Create Graph", { 0, 0, 200, 0 }, 24, { 0, 0, 0, 0 }, { 104, 38, 0, 0 }, { 20, 20, 64, 64 }, { 0, 200, 280, 100 }, switchToolToCGraph);
		FillButton = new Button(renderer, "", "", { 128, 128, 128, 0 }, 24, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 300, 280, 420 }, switchToolToNone);
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
	while (currentStep != Step::QUIT) {
		handler(renderer, &currentStep, AddRectangleButton, RmvRectangleButton, GraphRectangleButton, FillButton);
	}

	delete AddRectangleButton;
	AddRectangleButton = NULL;
	delete RmvRectangleButton;
	RmvRectangleButton = NULL;
	delete GraphRectangleButton;
	GraphRectangleButton = NULL;
	delete FillButton;
	FillButton = NULL;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();

	_CrtDumpMemoryLeaks();
	system("pause");
	return EXIT_SUCCESS;
}
