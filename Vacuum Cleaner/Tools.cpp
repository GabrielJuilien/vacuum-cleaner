#include "Tools.h"

int switchToolToDraw(void* input) {
	Tool* currentTool = static_cast<Tool*>(input);
	*currentTool = Tool::DRAW;
	std::cout << "Switched tool to DRAW mode.\n";
	return 0;
}

int switchToolToErase(void* input) {
	Tool* currentTool = static_cast<Tool*>(input);
	*currentTool = Tool::ERASE;
	std::cout << "Switched tool to ERASE mode.\n";
	return 0;
}

int switchToolToNone(void* input) {
	Tool* currentTool = static_cast<Tool*>(input);
	*currentTool = Tool::NONE;
	std::cout << "Switched tool to NONE mode.\n";
	return 0;
}

int switchToolToCGraph(void* input) {
	Tool* currentTool = static_cast<Tool*>(input);
	*currentTool = Tool::CGRAPH;
	std::cout << "Switched tool to NONE mode.\n";
	return 0;
}

void handler(SDL_Renderer* p_renderer, Step* currentStep, Button* AddRectangleButton, Button* RmvRectangleButton, Button* GraphRectangleButton, Button* FillButton) {

	int a, b;
	std::string s_a, s_b;

	static SDL_Event e;
	static int x_mousePos, y_mousePos;
	static bool clickLock = false;

	static Tool* currentTool = new Tool(Tool::NONE);

	static View* view = new View(p_renderer, { 0, 0, 1020, 20, false }, { 0, 0, 20, 720, false }, { 20, 20, 1000, 700, false });

	static Rect* drawingBuffer = NULL;
	static Text* drawingTextX = new Text("", p_renderer, { 0, 0, 0, 0 }, 16, 0, 0);
	static Text* drawingTextY = new Text("", p_renderer, { 0, 0, 0, 0 }, 16, 0, 0);

	static int decimals;

	SDL_GetMouseState(&x_mousePos, &y_mousePos);

	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		/*------Mouse events------*/
		case SDL_MOUSEBUTTONDOWN:
			clickLock = true;
			if (e.button.button == SDL_BUTTON_LEFT) {
				if (AddRectangleButton->trigger(x_mousePos, y_mousePos, currentTool)) break;
				else if (RmvRectangleButton->trigger(x_mousePos, y_mousePos, currentTool)) break;
				else if (GraphRectangleButton->trigger(x_mousePos, y_mousePos, NULL)) break;
				else {
					if (*currentTool == Tool::DRAW) {
						if (!view->drawingBuffer()) {
							view->setBufferOrigin(x_mousePos, y_mousePos, true, 280, 20);
						}
						else {
							view->setBufferTarget(x_mousePos, y_mousePos, 280, 20);
							view->validateBuffer();
						}
					}
					else if (*currentTool == Tool::ERASE) {
						if (!view->drawingBuffer()) {
							view->setBufferOrigin(x_mousePos, y_mousePos, false, 280, 20);
						}
						else {
							view->setBufferTarget(x_mousePos, y_mousePos, 280, 20);
							view->validateBuffer();
						}
					}
				}
			}
			else if (e.button.button == SDL_BUTTON_MIDDLE) {

			}
			else if (e.button.button == SDL_BUTTON_RIGHT) {
				view->discardBuffer();
				switchToolToNone(currentTool);
			}
			break;
		case SDL_MOUSEBUTTONUP:
			clickLock = false;
			break;
		case SDL_MOUSEMOTION:
			break;
		case SDL_MOUSEWHEEL:
			if (e.wheel.y > 0) view->zoom(1, 0, 0);
			else view->zoom(-1, 0, 0);
			view->updateScales(p_renderer, 260, 0);
			break;
		/*-----------------------*/
		/*----Keyboard events----*/
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
			case SDLK_UP:
				break;
			case SDLK_DOWN:
				break;
			case SDLK_LEFT:
				break;
			case SDLK_RIGHT:
				break;
			case SDLK_SPACE:
				break;
			case SDLK_ESCAPE:
				break;
			}
			break;
		/*-----------------------*/
		/*-----Window events-----*/
		case SDL_QUIT:
			break;
		}
	}

	if (*currentStep != Step::QUIT) render(p_renderer, *currentStep, view, AddRectangleButton, RmvRectangleButton, GraphRectangleButton, FillButton);
	else {
		delete view;
		view = NULL;
		if (drawingBuffer) delete drawingBuffer;
		drawingBuffer = NULL;
		delete drawingTextX;
		drawingTextX = NULL;
		delete drawingTextY;
		drawingTextY = NULL;
		delete currentTool;
		currentTool = NULL;
	}
}

void render(SDL_Renderer* p_renderer, Step currentStep, View* view, Button* AddRectangleButton, Button* RmvRectangleButton, Button* GraphRectangleButton, Button* FillButton) {
	static int lastFrame = SDL_GetTicks(), currentFrame = SDL_GetTicks();

	currentFrame = SDL_GetTicks();

	//Render loop
	while (currentFrame - lastFrame > 1000 / FRAMERATE) {
		SDL_RenderClear(p_renderer);
		if (view) view->render(p_renderer, 260, 0);
		if (AddRectangleButton) AddRectangleButton->render();
		if (RmvRectangleButton) RmvRectangleButton->render();
		if (GraphRectangleButton) GraphRectangleButton->render();
		if (FillButton) FillButton->render();
		SDL_RenderPresent(p_renderer);
		lastFrame += 1000 / FRAMERATE;
	}

	//Idle wait
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
}