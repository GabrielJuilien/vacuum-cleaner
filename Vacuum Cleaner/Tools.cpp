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
		switch (*currentStep) {
		case Step::DRAW:
			switch (*currentTool)
			{
			case Tool::DRAW:
				switch (e.type) {
				case SDL_MOUSEMOTION:
					//Updating buffer's target position and metrics display
					if (drawingBuffer) {
						drawingBuffer->target({ x_mousePos, y_mousePos });

						drawingTextX->makeFromInt(abs(drawingBuffer->w()), { drawingBuffer->x() + drawingBuffer->w() / 2 - drawingTextX->x_size() / 2, drawingBuffer->y() });

						drawingTextY->makeFromInt(abs(drawingBuffer->h()) , { drawingBuffer->x(), drawingBuffer->y() + drawingBuffer->h() / 2 - drawingTextY->y_size() / 2 });
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (e.button.button == SDL_BUTTON_RIGHT) {
						if (!AddRectangleButton->trigger(x_mousePos, y_mousePos, (void*)currentTool) && !RmvRectangleButton->trigger(x_mousePos, y_mousePos, (void*)currentTool) && !FillButton->trigger(x_mousePos, y_mousePos, (void*)currentTool)) {
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
							if (!AddRectangleButton->trigger(x_mousePos, y_mousePos, (void*)currentTool) && !RmvRectangleButton->trigger(x_mousePos, y_mousePos, (void*)currentTool) && !FillButton->trigger(x_mousePos, y_mousePos, (void*)currentTool)) {
								if (!drawingBuffer) {
									drawingBuffer = new Rect(x_mousePos, y_mousePos, 0, 0, true);
								}
								else {
									//Pushing buffer to the stack and setting it to NULL
									view->drawing()->push_back(drawingBuffer);
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

							drawingTextX->makeFromInt(abs(drawingBuffer->w()), { drawingBuffer->x() + drawingBuffer->w() / 2 - drawingTextX->x_size() / 2, drawingBuffer->y() });
						}
						break;
					case SDLK_RIGHT:
						if (drawingBuffer && drawingBuffer->x() + drawingBuffer->w() < 1280) {
							drawingBuffer->w(drawingBuffer->w() + 1);

							drawingTextX->makeFromInt(abs(drawingBuffer->w()), { drawingBuffer->x() + drawingBuffer->w() / 2 - drawingTextX->x_size() / 2, drawingBuffer->y() });
						}
						break;
					case SDLK_UP:
						if (drawingBuffer && drawingBuffer->y() + drawingBuffer->h() >= 0) {
							drawingBuffer->h(drawingBuffer->h() - 1);

							drawingTextY->makeFromInt(abs(drawingBuffer->h()), { drawingBuffer->x(), drawingBuffer->y() + drawingBuffer->h() / 2 - drawingTextY->y_size() / 2 });
						}
						break;
					case SDLK_DOWN:
						if (drawingBuffer && drawingBuffer->y() + drawingBuffer->h() < 720) {
							drawingBuffer->h(drawingBuffer->h() + 1);

							drawingTextY->makeFromInt(abs(drawingBuffer->h()), { drawingBuffer->x(), drawingBuffer->y() + drawingBuffer->h() / 2 - drawingTextY->y_size() / 2 });
						}
						break;
					case SDLK_SPACE:
						if (!drawingBuffer) {
							drawingBuffer = new Rect(x_mousePos, y_mousePos, 0, 0, true);
						}
						else {
							//Pushing buffer to the stack and setting it to NULL
							view->drawing()->push_back(drawingBuffer);
							drawingBuffer = NULL;
							drawingTextX->text("");
							drawingTextY->text("");
						}
						break;
					}
					break;
				case SDL_MOUSEWHEEL:
					while (e.wheel.y > 0) {
						view->zoom(ZOOM_STEP, x_mousePos, y_mousePos);
						e.wheel.y--;
					}
					while (e.wheel.y < 0) {
						view->zoom(-ZOOM_STEP, x_mousePos, y_mousePos);
						e.wheel.y++;
					}
					view->updateScales(p_renderer, 260, 0);
					break;
				case SDL_QUIT:
					*currentStep = Step::QUIT;
					break;
				}
				break;
			case Tool::ERASE:
				switch (e.type) {
				case SDL_MOUSEMOTION:
					//Updating buffer's target position and metrics display
					if (drawingBuffer) {
						drawingBuffer->target({ x_mousePos, y_mousePos });

						drawingTextX->makeFromInt(abs(drawingBuffer->w()), { drawingBuffer->x() + drawingBuffer->w() / 2 - drawingTextX->x_size() / 2, drawingBuffer->y() });

						drawingTextY->makeFromInt(abs(drawingBuffer->h()), { drawingBuffer->x(), drawingBuffer->y() + drawingBuffer->h() / 2 - drawingTextY->y_size() / 2 });
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (e.button.button == SDL_BUTTON_RIGHT) {
						if (!AddRectangleButton->trigger(x_mousePos, y_mousePos, (void*)currentTool) && !RmvRectangleButton->trigger(x_mousePos, y_mousePos, (void*)currentTool) && !FillButton->trigger(x_mousePos, y_mousePos, (void*)currentTool)) {
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
							if (!AddRectangleButton->trigger(x_mousePos, y_mousePos, (void*)currentTool) && !RmvRectangleButton->trigger(x_mousePos, y_mousePos, (void*)currentTool) && !FillButton->trigger(x_mousePos, y_mousePos, (void*)currentTool)) {
								if (!drawingBuffer) {
									drawingBuffer = new Rect(x_mousePos, y_mousePos, 0, 0, false);
								}
								else {
									//Pushing buffer to the stack and setting it to NULL
									view->drawing()->push_back(drawingBuffer);
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

							drawingTextX->makeFromInt(abs(drawingBuffer->w()), { drawingBuffer->x() + drawingBuffer->w() / 2 - drawingTextX->x_size() / 2, drawingBuffer->y() });
						}
						break;
					case SDLK_RIGHT:
						if (drawingBuffer && drawingBuffer->x() + drawingBuffer->w() < 1280) {
							drawingBuffer->w(drawingBuffer->w() + 1);

							drawingTextX->makeFromInt(abs(drawingBuffer->w()), { drawingBuffer->x() + drawingBuffer->w() / 2 - drawingTextX->x_size() / 2, drawingBuffer->y() });
						}
						break;
					case SDLK_UP:
						if (drawingBuffer && drawingBuffer->y() + drawingBuffer->h() >= 0) {
							drawingBuffer->h(drawingBuffer->h() - 1);

							drawingTextY->makeFromInt(abs(drawingBuffer->h()), { drawingBuffer->x(), drawingBuffer->y() + drawingBuffer->h() / 2 - drawingTextY->y_size() / 2 });
						}
						break;
					case SDLK_DOWN:
						if (drawingBuffer && drawingBuffer->y() + drawingBuffer->h() < 720) {
							drawingBuffer->h(drawingBuffer->h() + 1);

							drawingTextY->makeFromInt(abs(drawingBuffer->h()), { drawingBuffer->x(), drawingBuffer->y() + drawingBuffer->h() / 2 - drawingTextY->y_size() / 2 });
						}
						break;
					case SDLK_SPACE:
						if (!drawingBuffer) {
							drawingBuffer = new Rect(x_mousePos, y_mousePos, 0, 0, false);
						}
						else {
							//Pushing buffer to the stack and setting it to NULL
							view->drawing()->push_back(drawingBuffer);
							drawingBuffer = NULL;
							drawingTextX->text("");
							drawingTextY->text("");
						}
						break;
					}
					break;
				case SDL_MOUSEWHEEL:
					while (e.wheel.y > 0) {
						view->zoom(ZOOM_STEP, x_mousePos, y_mousePos);
						e.wheel.y--;
					}
					while (e.wheel.y < 0) {
						view->zoom(-ZOOM_STEP, x_mousePos, y_mousePos);
						e.wheel.y++;
					}
					view->updateScales(p_renderer, 260, 0);
					break;
				case SDL_QUIT:
					*currentStep = Step::QUIT;
					break;
				}
				break;
				break;
			case Tool::NONE:
				switch (e.type) {
				case SDL_MOUSEBUTTONDOWN:
					AddRectangleButton->trigger(x_mousePos, y_mousePos, (void*)currentTool);
					RmvRectangleButton->trigger(x_mousePos, y_mousePos, (void*)currentTool);
					break;
				case SDL_MOUSEWHEEL:
					while (e.wheel.y > 0) {
						view->zoom(ZOOM_STEP, x_mousePos, y_mousePos);
						e.wheel.y--;
					}
					while (e.wheel.y < 0) {
						view->zoom(-ZOOM_STEP, x_mousePos, y_mousePos);
						e.wheel.y++;
					}
					view->updateScales(p_renderer, 260, 0);
					break;
				case SDL_QUIT:
					*currentStep = Step::QUIT;
					break;
				}
				break;
			}
			break;
		}
	}

	if (*currentStep != Step::QUIT) render(p_renderer, *currentStep, drawingBuffer, view, drawingTextX, drawingTextY, AddRectangleButton, RmvRectangleButton, GraphRectangleButton, FillButton);
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

void render(SDL_Renderer* p_renderer, Step currentStep, Rect* drawingBuffer, View* view, Text* drawingTextX, Text* drawingTextY, Button* AddRectangleButton, Button* RmvRectangleButton, Button* GraphRectangleButton, Button* FillButton) {
	static int lastFrame = SDL_GetTicks(), currentFrame = SDL_GetTicks();

	currentFrame = SDL_GetTicks();

	//Render loop
	while (currentFrame - lastFrame > 1000 / FRAMERATE) {
		SDL_RenderClear(p_renderer);
		if (AddRectangleButton) AddRectangleButton->render();
		if (RmvRectangleButton) RmvRectangleButton->render();
		if (GraphRectangleButton) GraphRectangleButton->render();
		if (FillButton) FillButton->render();
		if (view) view->render(p_renderer, 260, 0);
		if (drawingBuffer) drawingBuffer->render(p_renderer, { 150, 150, 220, 100 });
		if (drawingTextX) drawingTextX->render(0, 0);
		if (drawingTextY) drawingTextY->render(0, 0);
		SDL_RenderPresent(p_renderer);
		lastFrame += 1000 / FRAMERATE;
	}

	//Idle wait
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
}