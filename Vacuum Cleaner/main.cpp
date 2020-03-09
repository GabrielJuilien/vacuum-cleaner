
#include "utils.h"
#include "Button.h"
#include "Tools.h"
#include "Rect.h"


enum class Step {
	HOME,
	DRAW,
	SIMULATION_LOADING,
	SIMULATION_RENDERING,
	QUIT
};


int main(int argc, char** argv)
{
	int a, b, c, d;
	std::string s_a, s_b;
	SDL_Window* window;
	SDL_Renderer* renderer;

	Step currentStep = Step::DRAW;
	SDL_Event e;
	int x_mousePos = 0, y_mousePos = 0;
	int lastFrame = SDL_GetTicks(), currentFrame = SDL_GetTicks();

	Button* AddRectangleButton = NULL;
	Button* RmvRectangleButton = NULL;

	bool clickLock = false;

	Tool* currentTool = new Tool(Tool::NONE);
	std::vector<Rect*>* drawing = new std::vector<Rect*>();
	Rect* drawingBuffer = NULL;
	Text* drawingTextX = NULL;
	Text* drawingTextY = NULL;

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
		RmvRectangleButton = new Button(renderer, "ressources/erase_rectangle.png", "Erase rectangle", { 200, 0, 0, 0 }, 24, { 0, 0, 0, 0 }, { 104, 38, 0, 0 }, { 20, 20, 64, 64 }, { 0, 100, 300, 100 }, switchToolToErase);
		drawingTextX = new Text("", renderer, { 0, 0, 0, 0 }, 16, 0, 0);
		drawingTextY = new Text("", renderer, { 0, 0, 0, 0 }, 16, 0, 0);	
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
				switch (*currentTool)
				{
				case Tool::DRAW:
					switch (e.type) {
					case SDL_MOUSEMOTION:
						//Updating buffer's target position and metrics display
						if (drawingBuffer) {
							drawingBuffer->target({ x_mousePos, y_mousePos });

							drawingTextX->destination(drawingBuffer->x() + drawingBuffer->w() / 2 - drawingTextX->x_size() / 2, drawingBuffer->y());
							a = abs(drawingBuffer->w());
							s_a = "";
							s_a.append(std::to_string((int)a * PX_SIZE / 100));
							s_a.append(",");
							s_a.append(std::to_string((int)a * PX_SIZE % 100));
							s_a.append("m");
							drawingTextX->text(s_a);

							drawingTextY->destination(drawingBuffer->x(), drawingBuffer->y() + drawingBuffer->h() / 2 - drawingTextY->y_size() / 2);
							b = abs(drawingBuffer->h());
							s_b = "";
							s_b.append(std::to_string((int)b * PX_SIZE / 100));
							s_b.append(",");
							s_b.append(std::to_string((int)b * PX_SIZE % 100));
							s_b.append("m"); 
							drawingTextY->text(s_b);
						}
					case SDL_MOUSEBUTTONDOWN:
						if (e.button.button == SDL_BUTTON_RIGHT) {
							if (!AddRectangleButton->trigger(x_mousePos, y_mousePos, (void*)currentTool) && !RmvRectangleButton->trigger(x_mousePos, y_mousePos, (void*)currentTool)) {
								switchToolToNone((void*)currentTool);
								if (drawingBuffer) {
									delete drawingBuffer;
									drawingBuffer = NULL;
									drawingTextX->text("");
									drawingTextY->text("");
								}
							}
							else {
								if (drawingBuffer) {
									delete drawingBuffer;
									drawingBuffer = NULL;
									drawingTextX->text("");
									drawingTextY->text("");
								}
							}
						}
						else if (e.button.button == SDL_BUTTON_LEFT) {
							if (!clickLock) {
								clickLock = true;
								if (!AddRectangleButton->trigger(x_mousePos, y_mousePos, (void*)currentTool) && !RmvRectangleButton->trigger(x_mousePos, y_mousePos, (void*)currentTool)) {
									if (!drawingBuffer) {
										drawingBuffer = new Rect(x_mousePos, y_mousePos, 0, 0);
									}
									else {
										//Pushing buffer to the stack and setting it to NULL
										drawing->push_back(drawingBuffer);
										drawingBuffer = NULL;
										drawingTextX->text("");
										drawingTextY->text("");
									}
								}
								else {
									if (drawingBuffer) {
										delete drawingBuffer;
										drawingBuffer = NULL;
										drawingTextX->text("");
										drawingTextY->text("");
									}
								}
							}
						}
						break;
					case SDL_MOUSEBUTTONUP:
						if (e.button.button == SDL_BUTTON_LEFT) {
							clickLock = false;
						}
						break;
					case SDL_KEYDOWN:
						switch (e.key.keysym.sym) {
						case SDLK_LEFT:
							if (drawingBuffer && drawingBuffer->x() + drawingBuffer->w() >= 0) {
								drawingBuffer->w(drawingBuffer->w() - 1);

								drawingTextX->destination(drawingBuffer->x() + drawingBuffer->w() / 2 - drawingTextX->x_size() / 2, drawingBuffer->y());
								a = abs(drawingBuffer->w());
								s_a = "";
								s_a.append(std::to_string((int)a* PX_SIZE / 100));
								s_a.append(",");
								s_a.append(std::to_string((int)a* PX_SIZE % 100));
								s_a.append("m");
								drawingTextX->text(s_a);
							}
							break;
						case SDLK_RIGHT:
							if (drawingBuffer && drawingBuffer->x() + drawingBuffer->w() < 1280) {
								drawingBuffer->w(drawingBuffer->w() + 1);

								drawingTextX->destination(drawingBuffer->x() + drawingBuffer->w() / 2 - drawingTextX->x_size() / 2, drawingBuffer->y());
								a = abs(drawingBuffer->w());
								s_a = "";
								s_a.append(std::to_string((int)a * PX_SIZE / 100));
								s_a.append(",");
								s_a.append(std::to_string((int)a * PX_SIZE % 100));
								s_a.append("m");
								drawingTextX->text(s_a);
							}
							break;
						case SDLK_UP:
							if (drawingBuffer && drawingBuffer->y() + drawingBuffer->h() >= 0) {
								drawingBuffer->h(drawingBuffer->h() - 1);

								drawingTextY->destination(drawingBuffer->x(), drawingBuffer->y() + drawingBuffer->h() / 2 - drawingTextY->y_size() / 2);
								b = abs(drawingBuffer->h());
								s_b = "";
								s_b.append(std::to_string((int)b* PX_SIZE / 100));
								s_b.append(",");
								s_b.append(std::to_string((int)b* PX_SIZE % 100));
								s_b.append("m");
								drawingTextY->text(s_b);
							}
							break;
						case SDLK_DOWN:
							if (drawingBuffer && drawingBuffer->y() + drawingBuffer->h() < 720) {
								drawingBuffer->h(drawingBuffer->h() + 1);

								drawingTextY->destination(drawingBuffer->x(), drawingBuffer->y() + drawingBuffer->h() / 2 - drawingTextY->y_size() / 2);
								b = abs(drawingBuffer->h());
								s_b = "";
								s_b.append(std::to_string((int)b* PX_SIZE / 100));
								s_b.append(",");
								s_b.append(std::to_string((int)b* PX_SIZE % 100));
								s_b.append("m");
								drawingTextY->text(s_b);
							}
							break;
						case SDLK_SPACE:
							if (!drawingBuffer) {
								drawingBuffer = new Rect(x_mousePos, y_mousePos, 0, 0);
							}
							else {
								//Pushing buffer to the stack and setting it to NULL
								drawing->push_back(drawingBuffer);
								drawingBuffer = NULL;
								drawingTextX->text("");
								drawingTextY->text("");
							}
							break;
						}
						break;
					case SDL_QUIT:
						currentStep = Step::QUIT;
						break;
					}
					break;
				case Tool::ERASE:
					switch (e.type) {
					case SDL_MOUSEBUTTONDOWN:
						if (e.button.button == SDL_BUTTON_RIGHT) {
							if (!AddRectangleButton->trigger(x_mousePos, y_mousePos, (void*)currentTool) && !RmvRectangleButton->trigger(x_mousePos, y_mousePos, (void*)currentTool)) {
								switchToolToNone((void*)currentTool);
								if (drawingBuffer) {
									delete drawingBuffer;
									drawingBuffer = NULL;
									drawingTextX->text("");
									drawingTextY->text("");
								}
							}
							else {
								if (drawingBuffer) {
									delete drawingBuffer;
									drawingBuffer = NULL;
									drawingTextX->text("");
									drawingTextY->text("");
								}
							}
						}
						else if (e.button.button == SDL_BUTTON_LEFT) {
							if (!clickLock) {
								clickLock = true;
								if (!AddRectangleButton->trigger(x_mousePos, y_mousePos, (void*)currentTool) && !RmvRectangleButton->trigger(x_mousePos, y_mousePos, (void*)currentTool)) {
									//Setting rectangle origin
								}
							}
						}
						break;
					case SDL_MOUSEBUTTONUP:
						if (e.button.button == SDL_BUTTON_LEFT) {
							clickLock = false;
						}
						break;
					case SDL_QUIT:
						currentStep = Step::QUIT;
						break;
					}
					break;
				case Tool::NONE:
					switch (e.type) {
					case SDL_MOUSEBUTTONDOWN:
						AddRectangleButton->trigger(x_mousePos, y_mousePos, (void*)currentTool);
						RmvRectangleButton->trigger(x_mousePos, y_mousePos, (void*)currentTool);
						break;
					case SDL_QUIT:
						currentStep = Step::QUIT;
						break;
					}
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
			for (int i = 0; i < drawing->size(); i++) drawing->at(i)->render(renderer, { 128, 128, 200, 0 });
			if (drawingBuffer) drawingBuffer->render(renderer, { 150, 150, 220, 100 });
			if (drawingTextX) drawingTextX->render(0, 0);
			if (drawingTextY) drawingTextY->render(0, 0);
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

	system("pause");
	return EXIT_SUCCESS;
}
