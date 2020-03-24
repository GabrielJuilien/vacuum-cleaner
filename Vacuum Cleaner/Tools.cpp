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
	static SDL_Event e;
	static int x_mousePos, y_mousePos;
	static bool clickLock = false;
	static bool wheelClick = false;

	static Tool* currentTool = new Tool(Tool::NONE);

	static View* view = new View(p_renderer, { 0, 0, 1020, 20, false }, { 0, 0, 20, 720, false }, { 20, 20, 1000, 700, false });

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
							view->validateBuffer();
						}
					}
					else if (*currentTool == Tool::ERASE) {
						if (!view->drawingBuffer()) {
							view->setBufferOrigin(x_mousePos, y_mousePos, false, 280, 20);
						}
						else {
							view->validateBuffer();
						}
					}
				}
			}
			else if (e.button.button == SDL_BUTTON_MIDDLE) {
				wheelClick = true;
			}
			else if (e.button.button == SDL_BUTTON_RIGHT) {
				view->discardBuffer();
				switchToolToNone(currentTool);
			}
			break;
		case SDL_MOUSEBUTTONUP:
			clickLock = false;	
			if (wheelClick) view->updateScales(p_renderer, 260, 0);
			wheelClick = false;
			break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&x_mousePos, &y_mousePos);
			if (view->drawingBuffer()) {
				view->setBufferTarget(x_mousePos, y_mousePos, 280, 20);
			}
			if (wheelClick) {
				view->moveCenter(-e.motion.xrel, -e.motion.yrel);
			}
			break;
		case SDL_MOUSEWHEEL:
			while (e.wheel.y > 0) {
				view->zoom(1.0f, 0, 0);
				e.wheel.y--;
			}
			if (e.wheel.y < 0) {
				view->moveCenter(view->viewer()->w() / 2 * PX_SIZE - view->viewCenter().x, view->viewer()->h() / 2 * PX_SIZE - view->viewCenter().y);
			}
			while (e.wheel.y < 0) {
				view->zoom(-1.0f, 0, 0);
				e.wheel.y++;
			}
			view->updateScales(p_renderer, 260, 0);
			break;
		/*-----------------------*/
		/*----Keyboard events----*/
		case SDL_KEYDOWN:
			int x, y;
			switch (e.key.keysym.sym) {
			case SDLK_UP:
				SDL_GetGlobalMouseState(&x, &y);
				SDL_WarpMouseGlobal(x, y - 1);
				SDL_GetMouseState(&x_mousePos, &y_mousePos);
				if (view->drawingBuffer()) {
					view->setBufferTarget(x_mousePos, y_mousePos, 280, 20);
				}
				break;
			case SDLK_DOWN:
				SDL_GetGlobalMouseState(&x, &y);
				SDL_WarpMouseGlobal(x, y + 1);
				SDL_GetMouseState(&x_mousePos, &y_mousePos);
				if (view->drawingBuffer()) {
					view->setBufferTarget(x_mousePos, y_mousePos, 280, 20);
				}
				break;
			case SDLK_LEFT:
				SDL_GetGlobalMouseState(&x, &y);
				SDL_WarpMouseGlobal(x - 1, y);
				SDL_GetMouseState(&x_mousePos, &y_mousePos);
				if (view->drawingBuffer()) {
					view->setBufferTarget(x_mousePos, y_mousePos, 280, 20);
				}
				break;
			case SDLK_RIGHT:
				SDL_GetGlobalMouseState(&x, &y);
				SDL_WarpMouseGlobal(x + 1, y);
				SDL_GetMouseState(&x_mousePos, &y_mousePos);
				if (view->drawingBuffer()) {
					view->setBufferTarget(x_mousePos, y_mousePos, 280, 20);
				}
				break;
			case SDLK_SPACE:
				SDL_GetMouseState(&x_mousePos, &y_mousePos);
				if (view->drawingBuffer()) {
					view->validateBuffer();
				}
				else {
					if (*currentTool == Tool::DRAW) {
						view->setBufferOrigin(x_mousePos, y_mousePos, true, 280, 20);
					}
					else if (*currentTool == Tool::ERASE) {
						view->setBufferOrigin(x_mousePos, y_mousePos, false, 280, 20);
					}
				}
				break;
			case SDLK_ESCAPE:
				*currentStep = Step::QUIT;
				break;
			}
			break;
		/*-----------------------*/
		/*-----Window events-----*/
		case SDL_QUIT:
			*currentStep = Step::QUIT;
			break;
		}
		/*-----------------------*/
	}

	if (*currentStep != Step::QUIT) render(p_renderer, *currentStep, view, AddRectangleButton, RmvRectangleButton, GraphRectangleButton, FillButton);
	else {
		delete view;
		view = NULL;
		delete currentTool;
		currentTool = NULL;
	}
}

void render(SDL_Renderer* p_renderer, Step currentStep, View* view, Button* AddRectangleButton, Button* RmvRectangleButton, Button* GraphRectangleButton, Button* FillButton) {
	static int lastFrame = SDL_GetTicks(), currentFrame = SDL_GetTicks();
	static int x, y;
	currentFrame = SDL_GetTicks();

	//Render loop
	while (currentFrame - lastFrame > 1000 / FRAMERATE) {
		SDL_RenderClear(p_renderer);
		if (view) {
			view->render(p_renderer, 260, 0);
		}
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