#include "Button.h"
#include "Tools.h"
#include "Rect.h"

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

void handler(SDL_Renderer* p_renderer, Step currentStep, Button* AddRectangleButton, Button* RmvRectangleButton, Button* FillButton) {

	int a, b, c, d;
	std::string s_a, s_b;

	static SDL_Event e;
	static int x_mousePos, y_mousePos;
	static bool clickLock = false;

	static Tool* currentTool = new Tool(Tool::NONE);

	static std::vector<Rect*>* drawing = new std::vector<Rect*>();
	static Rect* drawingBuffer = NULL;
	static Text* drawingTextX = new Text("", p_renderer, { 0, 0, 0, 0 }, 16, 0, 0);
	static Text* drawingTextY = new Text("", p_renderer, { 0, 0, 0, 0 }, 16, 0, 0);

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
							s_a.append(std::to_string((int)a * PX_SIZE / 100));
							s_a.append(",");
							s_a.append(std::to_string((int)a * PX_SIZE % 100));
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
							s_b.append(std::to_string((int)b * PX_SIZE / 100));
							s_b.append(",");
							s_b.append(std::to_string((int)b * PX_SIZE % 100));
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
							s_b.append(std::to_string((int)b * PX_SIZE / 100));
							s_b.append(",");
							s_b.append(std::to_string((int)b * PX_SIZE % 100));
							s_b.append("m");
							drawingTextY->text(s_b);
						}
						break;
					case SDLK_SPACE:
						if (!drawingBuffer) {
							drawingBuffer = new Rect(x_mousePos, y_mousePos, 0, 0, true);
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
					delete AddRectangleButton;
					delete RmvRectangleButton;
					delete FillButton;
					delete drawing;
					if (drawingBuffer) delete drawingBuffer;
					delete drawingTextX;
					delete drawingTextY;
					currentStep = Step::QUIT;
					break;
				}
				break;
			case Tool::ERASE:
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
							s_a.append(std::to_string((int)a * PX_SIZE / 100));
							s_a.append(",");
							s_a.append(std::to_string((int)a * PX_SIZE % 100));
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
							s_b.append(std::to_string((int)b * PX_SIZE / 100));
							s_b.append(",");
							s_b.append(std::to_string((int)b * PX_SIZE % 100));
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
							s_b.append(std::to_string((int)b * PX_SIZE / 100));
							s_b.append(",");
							s_b.append(std::to_string((int)b * PX_SIZE % 100));
							s_b.append("m");
							drawingTextY->text(s_b);
						}
						break;
					case SDLK_SPACE:
						if (!drawingBuffer) {
							drawingBuffer = new Rect(x_mousePos, y_mousePos, 0, 0, false);
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
					delete AddRectangleButton;
					delete RmvRectangleButton;
					delete FillButton;
					delete drawing;
					if (drawingBuffer) delete drawingBuffer;
					delete drawingTextX;
					delete drawingTextY;
					currentStep = Step::QUIT;
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
				case SDL_QUIT:
					delete AddRectangleButton;
					delete RmvRectangleButton;
					delete FillButton;
					delete drawing;
					if (drawingBuffer) delete drawingBuffer;
					delete drawingTextX;
					delete drawingTextY;
					currentStep = Step::QUIT;
					break;
				}
				break;
			}
			break;
		}
	}

	if (currentStep != Step::QUIT) render(p_renderer, drawingBuffer, drawing, drawingTextX, drawingTextY, AddRectangleButton, RmvRectangleButton, FillButton);
}

void render(SDL_Renderer* p_renderer, Rect* drawingBuffer, std::vector<Rect*>* drawing, Text* drawingTextX, Text* drawingTextY, Button* AddRectangleButton, Button* RmvRectangleButton, Button* FillButton) {
	static int lastFrame = SDL_GetTicks(), currentFrame = SDL_GetTicks();
	
	currentFrame = SDL_GetTicks();

	//Render loop
	while (currentFrame - lastFrame > 1000 / FRAMERATE) {
		SDL_RenderClear(p_renderer);
		AddRectangleButton->render();
		RmvRectangleButton->render();
		FillButton->render();
		for (int i = 0; i < drawing->size(); i++) {
			if (drawing->at(i)->draw())
				drawing->at(i)->render(p_renderer, { 100, 100, 255, 0 });
			else
				drawing->at(i)->render(p_renderer, { 255, 255, 255, 0 });
		}
		if (drawingBuffer) drawingBuffer->render(p_renderer, { 150, 150, 220, 100 });
		if (drawingTextX) drawingTextX->render(0, 0);
		if (drawingTextY) drawingTextY->render(0, 0);
		SDL_RenderPresent(p_renderer);
		lastFrame += 1000 / FRAMERATE;
	}

	//Idle wait
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
}