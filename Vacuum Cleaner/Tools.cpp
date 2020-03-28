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
	//Loop variables
	int i, j, k; 

	//Temporary variables
	GraphNode* tmp_node = NULL;
	GraphNode* buffer = NULL;
	GraphNode* left_node = NULL;

	//Retrieving pointers from input
	GraphData* graphData = static_cast<GraphData*>(input);
	GraphNode** Graph = graphData->m_graph;
	std::vector<Rect*>* drawing = graphData->m_drawing;

	std::cout << "Creating graph ..." << std::endl;

	for (i = 0; i < 1000 * PX_SIZE; i += PX_SIZE) {
		for (j = 0; j < 700 * PX_SIZE; j += PX_SIZE) {
			tmp_node = new GraphNode(NodeType::null, NodeState::null, NULL, NULL, NULL, NULL, i, j);
			if (i == 0 && j == 0) *Graph = tmp_node;

			//Linking left node
			if (i > 0) {
				left_node = (*Graph)->seekGraph(i / PX_SIZE - 1, j / PX_SIZE);
				tmp_node->lft(left_node);
				left_node->rgt(tmp_node);
			}
			//Linking upper node
			if (j > 0) {
				tmp_node->top(buffer);
				buffer->bot(tmp_node);
			}
			buffer = tmp_node;
			for (k = drawing->size() - 1; k > 0; k--) {
				if (drawing->at(k)->x() <= tmp_node->x() && drawing->at(k)->x() + drawing->at(k)->w() >= tmp_node->x() && drawing->at(k)->y() <= tmp_node->y() && drawing->at(k)->y() + drawing->at(k)->h() >= tmp_node->y()) {
					if (drawing->at(k)->draw())
						tmp_node->type(NodeType::floor);
					break;
				}
			}
		}
	}

	std::cout << "Done." << std::endl;

	return 0;
}

void handler(SDL_Renderer* p_renderer, Step* currentStep, Button* AddRectangleButton, Button* RmvRectangleButton, Button* GraphButton, Button* FillButton, View* p_view, GraphData* p_graphData) {
	static SDL_Event e;

	static int x_mousePos, y_mousePos;
	static float x_mouseBuffer, y_mouseBuffer;

	static bool clickLock = false;
	static bool wheelClick = false;

	static Tool* currentTool = new Tool(Tool::NONE);

	SDL_GetMouseState(&x_mousePos, &y_mousePos);

	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		/*------Mouse events------*/
		case SDL_MOUSEBUTTONDOWN:
			clickLock = true;
			if (e.button.button == SDL_BUTTON_LEFT) {
				if (AddRectangleButton->trigger(x_mousePos, y_mousePos, currentTool)) break;
				else if (RmvRectangleButton->trigger(x_mousePos, y_mousePos, currentTool)) break;
				else if (GraphButton->trigger(x_mousePos, y_mousePos, p_graphData)) break;
				else {
					if (*currentTool == Tool::DRAW) {
						if (!p_view->drawingBuffer()) {
							p_view->setBufferOrigin(x_mousePos, y_mousePos, true, 280, 20);
						}
						else {
							p_view->validateBuffer();
						}
					}
					else if (*currentTool == Tool::ERASE) {
						if (!p_view->drawingBuffer()) {
							p_view->setBufferOrigin(x_mousePos, y_mousePos, false, 280, 20);
						}
						else {
							p_view->validateBuffer();
						}
					}
				}
			}
			else if (e.button.button == SDL_BUTTON_MIDDLE) {
				wheelClick = true;
				x_mouseBuffer = x_mousePos;
				y_mouseBuffer = y_mousePos;
			}
			else if (e.button.button == SDL_BUTTON_RIGHT) {
				p_view->discardBuffer();
				switchToolToNone(currentTool);
			}
			break;
		case SDL_MOUSEBUTTONUP:
			clickLock = false;	
			if (wheelClick) {
				wheelClick = false;
				p_view->moveCenter(x_mouseBuffer - x_mousePos, y_mouseBuffer - y_mousePos);
				p_view->updateScales(p_renderer, 260, 0);
			}
			break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&x_mousePos, &y_mousePos);
			if (p_view->drawingBuffer()) {
				p_view->setBufferTarget(x_mousePos, y_mousePos, 280, 20);
			}
			if (wheelClick) {
				p_view->moveCenter(x_mouseBuffer - x_mousePos, y_mouseBuffer - y_mousePos);
				x_mouseBuffer = x_mousePos;
				y_mouseBuffer = y_mousePos;
			}
			break;
		case SDL_MOUSEWHEEL:
			while (e.wheel.y > 0) {
				p_view->zoom(1.0f, 0, 0);
				e.wheel.y--;
			}
			if (e.wheel.y < 0) {
				p_view->moveCenter(p_view->viewer()->w() / 2 * PX_SIZE - p_view->viewCenter().x, p_view->viewer()->h() / 2 * PX_SIZE - p_view->viewCenter().y);
			}
			while (e.wheel.y < 0) {
				p_view->zoom(-1.0f, 0, 0);
				e.wheel.y++;
			}
			p_view->updateScales(p_renderer, 260, 0);
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
				if (p_view->drawingBuffer()) {
					p_view->setBufferTarget(x_mousePos, y_mousePos, 280, 20);
				}
				break;
			case SDLK_DOWN:
				SDL_GetGlobalMouseState(&x, &y);
				SDL_WarpMouseGlobal(x, y + 1);
				SDL_GetMouseState(&x_mousePos, &y_mousePos);
				if (p_view->drawingBuffer()) {
					p_view->setBufferTarget(x_mousePos, y_mousePos, 280, 20);
				}
				break;
			case SDLK_LEFT:
				SDL_GetGlobalMouseState(&x, &y);
				SDL_WarpMouseGlobal(x - 1, y);
				SDL_GetMouseState(&x_mousePos, &y_mousePos);
				if (p_view->drawingBuffer()) {
					p_view->setBufferTarget(x_mousePos, y_mousePos, 280, 20);
				}
				break;
			case SDLK_RIGHT:
				SDL_GetGlobalMouseState(&x, &y);
				SDL_WarpMouseGlobal(x + 1, y);
				SDL_GetMouseState(&x_mousePos, &y_mousePos);
				if (p_view->drawingBuffer()) {
					p_view->setBufferTarget(x_mousePos, y_mousePos, 280, 20);
				}
				break;
			case SDLK_SPACE:
				SDL_GetMouseState(&x_mousePos, &y_mousePos);
				if (p_view->drawingBuffer()) {
					p_view->validateBuffer();
				}
				else {
					if (*currentTool == Tool::DRAW) {
						p_view->setBufferOrigin(x_mousePos, y_mousePos, true, 280, 20);
					}
					else if (*currentTool == Tool::ERASE) {
						p_view->setBufferOrigin(x_mousePos, y_mousePos, false, 280, 20);
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

	if (*currentStep != Step::QUIT) render(p_renderer, *currentStep, p_view, AddRectangleButton, RmvRectangleButton, GraphButton, FillButton);
	else {
		delete p_view;
		p_view = NULL;
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