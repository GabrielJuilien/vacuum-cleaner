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

int switchToolToPlaceRobot(void* input) {
	Tool* currentTool = static_cast<Tool*>(input);
	*currentTool = Tool::ROBOT;
	std::cout << "Switch tool to PLACE ROBOT.\n";
	return 0;
}

int switchToolToNone(void* input) {
	Tool* currentTool = static_cast<Tool*>(input);
	*currentTool = Tool::NONE;
	std::cout << "Switched tool to NONE mode.\n";
	return 0;
}

void createGraph(void* input) {
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
}

void createRobot(void* input) {
	GraphData* graphData = static_cast<GraphData*>(input);
	Robot** robot = graphData->m_robot;

	*robot = new Robot(1000, 700);
}

int switchToolToCGraph(void* input) {
	int i, j;

	std::thread thread1(createGraph, input);
	std::thread thread2(createRobot, input);
	thread1.join();
	thread2.join();

	//Linking two graphs
	GraphData* graphData = static_cast<GraphData*>(input);
	(*(graphData->m_robot))->currentPosition((*(graphData->m_robot))->graph()->seekGraph(graphData->m_startingPoint.x / PX_SIZE, graphData->m_startingPoint.y / PX_SIZE));
	(*(graphData->m_robot))->currentPosition()->graphNode((*(graphData->m_graph))->seekGraph(graphData->m_startingPoint.x / PX_SIZE, graphData->m_startingPoint.y / PX_SIZE));

	for (i = 0; i < 15; i++) {
		for (j = 0; j < 15; j++) {
			(*(graphData->m_robot))->currentPosition()->seekGraph(i - 7, j - 7)->graphNode((*(graphData->m_robot))->currentPosition()->graphNode()->seekGraph(- 7 + i, - 7 + j));
			(*(graphData->m_robot))->currentPosition()->seekGraph(i - 7, j - 7)->graphNode()->state(NodeState::cleaned);
		}
	}

	return 0;
}

void handler(SDL_Renderer* p_renderer, Step* currentStep, Button* AddRectangleButton, Button* RmvRectangleButton, Button* GraphButton, Button* SetRobotPosButton, Button* FillButton, View* p_view, GraphData* p_graphData) {
	static SDL_Event e;

	static int x_mousePos, y_mousePos;
	static float x_mouseBuffer, y_mouseBuffer;

	static bool clickLock = false;
	static bool wheelClick = false;

	static Tool* currentTool = new Tool(Tool::NONE);

	SDL_GetMouseState(&x_mousePos, &y_mousePos);

	if (*currentStep == Step::DRAW) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				/*------Mouse events------*/
			case SDL_MOUSEBUTTONDOWN:
				clickLock = true;
				if (e.button.button == SDL_BUTTON_LEFT) {
					if (AddRectangleButton->trigger(x_mousePos, y_mousePos, currentTool)) break;
					else if (RmvRectangleButton->trigger(x_mousePos, y_mousePos, currentTool)) break;
					else if (SetRobotPosButton->trigger(x_mousePos, y_mousePos, currentTool)) break;
					else if (GraphButton->trigger(x_mousePos, y_mousePos, p_graphData)) {

						p_view->discardBuffer();

						delete p_view->xScale();
						p_view->xScale(NULL);
						delete p_view->yScale();
						p_view->yScale(NULL);

						p_view->x(140);
						p_view->y(10);

						*currentStep = Step::SIMULATION_RENDERING;
					}
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
						else if (*currentTool == Tool::ROBOT) {
							if (x_mousePos >= 288 && x_mousePos <= 1272 && y_mousePos && y_mousePos >= 28 && y_mousePos <= 712) {
								p_view->setRobotPosition(x_mousePos, y_mousePos);
								p_graphData->m_startingPoint = p_view->robotPosition();
								p_view->updateRobotImage();
								switchToolToNone(currentTool);
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
					p_view->updateScales(p_renderer, 0, 0);
				}
				break;
			case SDL_MOUSEMOTION:
				SDL_GetMouseState(&x_mousePos, &y_mousePos);
				if (p_view->drawingBuffer()) {
					p_view->setBufferTarget(x_mousePos, y_mousePos, 280, 20);
					p_view->updateXText(280, 20);
					p_view->updateYText(280, 20);

				}
				if (wheelClick) {
					p_view->moveCenter(x_mouseBuffer - x_mousePos, y_mouseBuffer - y_mousePos);
					x_mouseBuffer = x_mousePos;
					y_mouseBuffer = y_mousePos;
				}
				if (*currentTool == Tool::ROBOT) {
					p_view->setRobotPosition(x_mousePos, y_mousePos);
					p_view->updateRobotImage();
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
				p_view->updateScales(p_renderer, 0, 0);
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
					if (*currentTool == Tool::ROBOT) {
						if (x_mousePos >= 288 && x_mousePos <= 1272 && y_mousePos && y_mousePos >= 28 && y_mousePos <= 712) {
							p_view->setRobotPosition(x_mousePos, y_mousePos);
							p_graphData->m_startingPoint = p_view->robotPosition();
							p_view->updateRobotImage();
							switchToolToNone(currentTool);
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
	}
	else if (*currentStep == Step::SIMULATION_RENDERING) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				/*---Mouse events----*/
			case SDL_MOUSEBUTTONDOWN:
				break;;
				/*-------------------*/
				/*--Keyboard events--*/
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					*currentStep = Step::QUIT;
					break;
				}
				break;
				/*-------------------*/
				/*---Window events---*/
			case SDL_QUIT:
				*currentStep = Step::QUIT;
				break;
				/*-------------------*/
			}
		}
	}

	drawPhaseRender(p_renderer, currentStep, p_view, AddRectangleButton, RmvRectangleButton, GraphButton, SetRobotPosButton, FillButton);
}

void drawPhaseRender(SDL_Renderer* p_renderer, Step* currentStep, View* view, Button* AddRectangleButton, Button* RmvRectangleButton, Button* GraphRectangleButton, Button* SetRobotPosButton, Button* FillButton) {
	static int lastFrame = SDL_GetTicks(), currentFrame = SDL_GetTicks();
	static int x, y;
	static Image* robotImage = NULL;
	currentFrame = SDL_GetTicks();

	//Render loop
	while (currentFrame - lastFrame > 1000 / FRAMERATE) {
		SDL_RenderClear(p_renderer);
		if (view) view->render(p_renderer, 0, 0);
		if (AddRectangleButton) AddRectangleButton->render();
		if (RmvRectangleButton) RmvRectangleButton->render();
		if (GraphRectangleButton) GraphRectangleButton->render();
		if (SetRobotPosButton) SetRobotPosButton->render();
		if (FillButton) FillButton->render();
		SDL_RenderPresent(p_renderer);
		lastFrame += 1000 / FRAMERATE;
	}

	//Idle wait
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
}


void simulation(SDL_Renderer* p_renderer, Step* p_currentStep, GraphData* p_graphData) {
	int i;

	static Robot* p_robot = *(p_graphData->m_robot);
	
	static SDL_Event e;
	static bool update_view = true;

	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			*p_currentStep = Step::QUIT;
			break;
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
			case SDLK_SPACE:
				update_view = true;
				break;
			case SDLK_ESCAPE:
				*p_currentStep = Step::QUIT;
				break;
			}
		}
	}

	p_robot->evaluateStack();
	p_robot->sortStack();


	if (update_view)
		simulationPhaseRender(p_renderer, p_currentStep, p_graphData);
	else
		std::this_thread::sleep_for(std::chrono::milliseconds(5));

	update_view = false;
}

void simulationPhaseRender(SDL_Renderer* p_renderer, Step* p_currentStep, GraphData* p_graphData) {
	SDL_SetRenderDrawColor(p_renderer, 200, 200, 200, 0);
	SDL_RenderClear(p_renderer);

	int i, j;
 	RobotNode* tmp = NULL;
	RobotNode* tmp2 = NULL;

	static SDL_Texture* robotTexture = SDL_CreateTextureFromSurface(p_renderer, IMG_Load("ressources/robot.png"));
	static SDL_Rect destination = { 0, 0, 30, 30 };
	static SDL_Point center = { 15, 15 };

	tmp = (*(p_graphData->m_robot))->graph();
	for (i = 0; i < 1000; i++) {
		tmp2 = tmp->right();
		for (j = 0; j < 700; j++) {
			if (tmp->graphNode()) {
				switch (tmp->graphNode()->state())
				{
				case NodeState::null:
					SDL_SetRenderDrawColor(p_renderer, 255, 255, 255, 0);
					SDL_RenderDrawPoint(p_renderer, i + 280, j + 20);
					break;
				case NodeState::dirty:
					SDL_SetRenderDrawColor(p_renderer, 100, 100, 255, 0);
					SDL_RenderDrawPoint(p_renderer, i + 280, j + 20);
					break;
				case NodeState::cleaned:
					SDL_SetRenderDrawColor(p_renderer, 125, 125, 255, 0);
					SDL_RenderDrawPoint(p_renderer, i + 280, j + 20);
					break;
				default:
					break;
				}
			}
			tmp = tmp->bot();
		}
		tmp = tmp2;
	}

	destination = { (*(p_graphData->m_robot))->currentPosition()->graphNode()->x() / PX_SIZE + 280 - 7, (*(p_graphData->m_robot))->currentPosition()->graphNode()->y() / PX_SIZE + 20 - 7, 15, 15 };

	switch ((*(p_graphData->m_robot))->direction()) {
	case Direction::UP:
		SDL_RenderCopy(p_renderer, robotTexture, NULL, &destination);
		break;
	case Direction::RIGHT:
		SDL_RenderCopyEx(p_renderer, robotTexture, NULL, &destination, 90, &center, SDL_FLIP_NONE);
		break;
	case Direction::DOWN:
		SDL_RenderCopyEx(p_renderer, robotTexture, NULL, &destination, 180, &center, SDL_FLIP_NONE);
		break;
	case Direction::LEFT:
		SDL_RenderCopyEx(p_renderer, robotTexture, NULL, &destination, 270, &center, SDL_FLIP_NONE);
		break;
	}

	SDL_RenderPresent(p_renderer);
}

void deleteGraph(GraphNode* p_graph) {
	delete p_graph;
}

void deleteRobot(Robot* p_robot) {
	delete p_robot;
}