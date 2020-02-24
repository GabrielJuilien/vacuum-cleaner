#include "utils.h"

enum class Step {
	HOME,
	DRAW,
	SIMULATION_LOADING,
	SIMULATION_RENDERING,
	QUIT
};

int main(int argc, char** argv)
{
	SDL_Window* window;
	SDL_Renderer* renderer;

	SDL_Event e;

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
			return EXIT_FAILURE;
		}
	}

	window = SDL_CreateWindow("Vacuum Cleaner Planner", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);
	
	if (!window)
	{
		std::cerr << "[-] ERROR - Failed to create SDL window (" << SDL_GetError() << ")\n";
		return EXIT_FAILURE;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);



	//Main loop
	while (!SDL_QUIT) {

	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();

	return EXIT_SUCCESS;
}

