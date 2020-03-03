
#include "utils.h"
#include "Button.h"
#include "Tools.h"


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

	Step currentStep = Step::DRAW;
	SDL_Event e;
	int x_mousePos = 0, y_mousePos = 0;
	int lastFrame = SDL_GetTicks(), currentFrame = SDL_GetTicks();

	Button* AddRectangleButton = NULL;
	Button* RmvRectangleButton = NULL;

	Tool* currentTool = new Tool(Tool::NONE);

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
		AddRectangleButton = new Button(renderer, "ressources/draw_rectangle.png", "Draw rectangle", { 0, 150, 0, 0 }, 24, { 0, 0, 0, 0 }, { 104, 38, 0, 0 }, { 20, 20, 64, 64 }, { 0, 0, 300, 100 }, switchToolToDraw);
		RmvRectangleButton = new Button(renderer, "ressources/erase_rectangle.png", "Erase rectangle", { 200, 0, 0, 0 }, 24, { 0, 0, 0, 0 }, { 104, 38, 0, 0 }, { 20, 20, 64, 64 }, { 0, 100, 300, 100 }, NULL);
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
		SDL_GetMouseState(&x_mousePos, &y_mousePos);
		while (SDL_PollEvent(&e)) {
			switch (currentStep) {
			case Step::DRAW:
				switch (e.type) {
				case SDL_MOUSEBUTTONDOWN:
					if (!AddRectangleButton->trigger(x_mousePos, y_mousePos, (void*)currentTool) && !AddRectangleButton->trigger(x_mousePos, y_mousePos, (void*)currentTool)) {
						switchToolToNone((void*)currentTool);
					}
					break;
				case SDL_QUIT:
					currentStep = Step::QUIT;
					break;
				}
				break;
			}
			
		}

		currentFrame = SDL_GetTicks();

		//Render loop
		while (currentFrame - lastFrame > 1000 / FRAMERATE) {

			SDL_RenderClear(renderer);
			AddRectangleButton->render();
			RmvRectangleButton->render();
			SDL_RenderPresent(renderer);
			lastFrame += 1000 / FRAMERATE;
		}
		
		//Idle wait
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

	delete AddRectangleButton;
	delete RmvRectangleButton;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();

	return EXIT_SUCCESS;
}

