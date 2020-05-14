#include "Robot.h"

//Constructors
Robot::Robot() {
	m_targetNodesStack = new std::vector<RobotNode*>();
	m_targetZoneStack = new std::vector<RobotNode*>();
	m_currentPosition = NULL;
	m_graph = NULL;
	m_direction = Direction::UP;
}

Robot::Robot(int p_width, int p_height) {
	m_targetNodesStack = new std::vector<RobotNode*>();
	m_targetZoneStack = new std::vector<RobotNode*>();
	m_graph = NULL;
	m_currentPosition = NULL;
	
	int i, j;

	RobotNode* tmp = NULL;
	RobotNode* buffer = NULL;
	RobotNode* left_node = NULL;

	for (i = 0; i < p_width; i++) {
		for (j = 0; j < p_height; j++) {
			tmp = new RobotNode();
			if (i == 0 && j == 0) m_graph = tmp;

			//Linking left node
			if (i > 0) {
				left_node = m_graph->seekGraph(i - 1, j);
				tmp->left(left_node);
				left_node->right(tmp);
			}

			//Linking upper node
			if (j > 0) {
				tmp->top(buffer);
				buffer->bot(tmp);
			}

			buffer = tmp;
		}
	}

	m_direction = Direction::UP;
}

//Setters
void Robot::currentPosition(RobotNode* p_currentPos) {
	m_currentPosition = p_currentPos;
}

//Getters
RobotNode* Robot::currentPosition() {
	return m_currentPosition;
}

RobotNode* Robot::graph() {
	return m_graph;
}

Direction Robot::direction() {
	return m_direction;
}


//Movement management
float Robot::turn(Direction p_direction) {
	if (m_direction == p_direction) return 0.0f;
	switch (p_direction) {
	case Direction::UP:
		if (m_direction == Direction::DOWN) return 2.0f;
		else return 1.0f;
		break;
	case Direction::DOWN:
		if (m_direction == Direction::UP) return 2.0f;
		else return 1.0f;
		break;
	case Direction::LEFT:
		if (m_direction == Direction::RIGHT) return 2.0f;
		else return 1.0f;
		break;
	case Direction::RIGHT:
		if (m_direction == Direction::LEFT) return 2.0f;
		else return 1.0f;
		break;
	default:
		return 0.0f;
	}
}

bool Robot::forward(bool p_simulation) {
	int i;
	if (getFrontState()) return false;
	else {
		if (!p_simulation) {
			//Moving robot to the next node
			switch (m_direction)
			{
			case Direction::UP:
				m_currentPosition = m_currentPosition->top();
				for (i = 0; i < 15; i++) {
					removeNode(m_currentPosition->seekGraph(i - 7, -7));
				}
				break;
			case Direction::DOWN:
				m_currentPosition = m_currentPosition->bot();
				for (i = 0; i < 15; i++) {
					removeNode(m_currentPosition->seekGraph(i - 7, 7));
				}
				break;
			case Direction::LEFT:
				m_currentPosition = m_currentPosition->left();
				for (i = 0; i < 15; i++) {
					removeNode(m_currentPosition->seekGraph(- 7,i -7));
				}
				break;
			case Direction::RIGHT:
				m_currentPosition = m_currentPosition->right();
				for (i = 0; i < 15; i++) {
					removeNode(m_currentPosition->seekGraph(7,i - 7));
				}
				break;
			default:
				break;
			}
			
			//Retrieving captors state
			getFrontNodes();
			getLeftNodes();
			getRightNodes();
		}
		return true;
	}
}

bool Robot::backward(bool p_simulation) {
	if (getBackState()) return false;
	else {
		if (!p_simulation) {
			//Moving robot to the next node
			switch (m_direction)
			{
			case Direction::UP:
				m_currentPosition = m_currentPosition->bot();
				break;
			case Direction::DOWN:
				m_currentPosition = m_currentPosition->top();
				break;
			case Direction::LEFT:
				m_currentPosition = m_currentPosition->right();
				break;
			case Direction::RIGHT:
				m_currentPosition = m_currentPosition->left();
				break;
			default:
				break;
			}

			//Retrieving captors state
			getBackNodes();
			getLeftNodes();
			getRightNodes();
		}
		return true;
	}
}

//Captors state
unsigned short Robot::getFrontState() {
	int i;
	unsigned short res = 0;
	RobotNode* tmp = NULL;

	for (i = 0; i < 15; i++) {
		switch (m_direction){
		case Direction::UP:
			tmp = m_currentPosition->seekGraph(i - 7, -8);
			break;
		case Direction::DOWN:
			tmp = m_currentPosition->seekGraph(i - 7, 8);
			break;
		case Direction::LEFT:
			tmp = m_currentPosition->seekGraph(-8, i - 7);
			break;
		case Direction::RIGHT:
			tmp = m_currentPosition->seekGraph(8, i - 7);
			break;
		}

		if (tmp->graphNode() && tmp->graphNode()->type() == NodeType::null) res += 1;
		res = res << 1;
	}

	return res;
}

unsigned short Robot::getBackState() {
	int i;
	unsigned short res = 0;
	RobotNode* tmp = NULL;

	for (i = 0; i < 15; i++) {
		switch (m_direction) {
		case Direction::UP:
			tmp = m_currentPosition->seekGraph(i - 7, 8);
			break;
		case Direction::DOWN:
			tmp = m_currentPosition->seekGraph(i - 7, -8);
			break;
		case Direction::LEFT:
			tmp = m_currentPosition->seekGraph(8, i - 7);
			break;
		case Direction::RIGHT:
			tmp = m_currentPosition->seekGraph(-8, i - 7);
			break;
		}

		if (tmp->graphNode() && tmp->graphNode()->type() == NodeType::null) res += 1;
		res = res << 1;
	}

	return res;
}

unsigned short Robot::getRightState() {
	int i;
	unsigned short res = 0;
	RobotNode* tmp = NULL;

	for (i = 0; i < 15; i++) {
		switch (m_direction)
		{
		case Direction::UP:
			tmp = m_currentPosition->seekGraph(8, i - 7);
			break;
		case Direction::DOWN:
			tmp = m_currentPosition->seekGraph(-8, i - 7);
			break;
		case Direction::LEFT:
			tmp = m_currentPosition->seekGraph(i - 7, -8);
			break;
		case Direction::RIGHT:
			tmp = m_currentPosition->seekGraph(i - 7, 8);
			break;
		}

		if (tmp->graphNode() && tmp->graphNode()->type() == NodeType::null) res += 1;
		res = res << 1;
	}

	return res;
}

unsigned short Robot::getLeftState() {
	int i;
	unsigned short res = 0;
	RobotNode* tmp = NULL;

	for (i = 0; i < 15; i++) {
		switch (m_direction)
		{
		case Direction::UP:
			tmp = m_currentPosition->seekGraph(-8, i - 7);
			break;
		case Direction::DOWN:
			tmp = m_currentPosition->seekGraph(8, i - 7);
			break;
		case Direction::LEFT:
			tmp = m_currentPosition->seekGraph(i - 7, 8);
			break;
		case Direction::RIGHT:
			tmp = m_currentPosition->seekGraph(i - 7, -8);
			break;
		}

		if (tmp->graphNode() && tmp->graphNode()->type() == NodeType::null) res += 1;
		res = res << 1;
	}

	return res;
}

//Hitbox function
bool Robot::canStandOn(RobotNode* p_position) {
	int i, j;
	RobotNode* tmp;
	for (i = 0; i < 15; i++) {
		for (j = 0; j < 15; j++) {
			tmp = p_position->seekGraph(i - 7, j - 7);
			if (tmp->graphNode() == NULL || tmp->graphNode()->type() != NodeType::floor)
				return false;
		}
	}

	return true;
}

//Retrieve data from graph
void Robot::getFrontNodes() {
	int i;
	RobotNode* tmp;
	switch (m_direction) {
	case Direction::UP:
		tmp = m_currentPosition->seekGraph(-7, -7);
		for (i = 0; i < 15; i++) {
			tmp->top(tmp->graphNode()->top());
			if (tmp->graphNode()->top()->type() == NodeType::floor && tmp->graphNode()->top()->state() == NodeState::null) {
				tmp->top()->graphNode()->state(NodeState::dirty);
				addNode(tmp->top());
			}
			tmp = tmp->right();
		}
		break;
	case Direction::RIGHT:
		tmp = m_currentPosition->seekGraph(7, -7);
		for (i = 0; i < 15; i++) {
			tmp->right(tmp->graphNode()->rgt());
			if (tmp->graphNode()->rgt()->type() == NodeType::floor && tmp->graphNode()->rgt()->state() == NodeState::null) {
				tmp->right()->graphNode()->state(NodeState::dirty);
				addNode(tmp->right());
			}
			tmp = tmp->bot();
		}
		break;
	case Direction::DOWN:
		tmp = m_currentPosition->seekGraph(7, 7);
		for (i = 0; i < 15; i++) {
			tmp->bot(tmp->graphNode()->bot());
			if (tmp->graphNode()->bot()->type() == NodeType::floor && tmp->graphNode()->bot()->state() == NodeState::null) {
				tmp->graphNode()->bot()->state(NodeState::dirty);
				addNode(tmp->bot());
			}
			tmp = tmp->left();
		}
		break;
	case Direction::LEFT:
		tmp = m_currentPosition->seekGraph(-7, 7);
		for (i = 0; i < 15; i++) {
			tmp->left(tmp->graphNode()->lft());
			if (tmp->graphNode()->lft()->type() == NodeType::floor && tmp->graphNode()->lft()->state() == NodeState::null) {
				tmp->graphNode()->lft()->state(NodeState::dirty);
				addNode(tmp->left());
			}
			tmp = tmp->top();
		}
		break;
	}
}

void Robot::getBackNodes() {
	int i;
	RobotNode* tmp;
	switch (m_direction) {
	case Direction::DOWN:
		tmp = m_currentPosition->seekGraph(-7, -7);
		for (i = 0; i < 15; i++) {
			tmp->top(tmp->graphNode()->top());
			if (tmp->graphNode()->top()->type() == NodeType::floor && tmp->graphNode()->top()->state() == NodeState::null) {
				tmp->graphNode()->top()->state(NodeState::dirty);
				addNode(tmp->top());
			}
			tmp = tmp->right();
		}
		break;
	case Direction::LEFT:
		tmp = m_currentPosition->seekGraph(7, -7);
		for (i = 0; i < 15; i++) {
			tmp->right(tmp->graphNode()->rgt());
			if (tmp->graphNode()->rgt()->type() == NodeType::floor && tmp->graphNode()->rgt()->state() == NodeState::null) {
				tmp->graphNode()->rgt()->state(NodeState::dirty);
				addNode(tmp->right());
			}
			tmp = tmp->bot();
		}
		break;
	case Direction::UP:
		tmp = m_currentPosition->seekGraph(7, 7);
		for (i = 0; i < 15; i++) {
			tmp->bot(tmp->graphNode()->bot());
			if (tmp->graphNode()->bot()->type() == NodeType::floor && tmp->graphNode()->bot()->state() == NodeState::null) {
				tmp->graphNode()->bot()->state(NodeState::dirty);
				addNode(tmp->bot());
			}
			tmp = tmp->left();
		}
		break;
	case Direction::RIGHT:
		tmp = m_currentPosition->seekGraph(-7, 7);
		for (i = 0; i < 15; i++) {
			tmp->left(tmp->graphNode()->lft());
			if (tmp->graphNode()->lft()->type() == NodeType::floor && tmp->graphNode()->lft()->state() == NodeState::null) {
				tmp->graphNode()->lft()->state(NodeState::dirty);
				addNode(tmp->left());
			}
			tmp = tmp->top();
		}
		break;
	}
}

void Robot::getLeftNodes() {
	int i;
	RobotNode* tmp;
	switch (m_direction) {
	case Direction::RIGHT:
		tmp = m_currentPosition->seekGraph(-7, -7);
		for (i = 0; i < 15; i++) {
			tmp->top(tmp->graphNode()->top());
			if (tmp->graphNode()->top()->type() == NodeType::floor && tmp->graphNode()->top()->state() == NodeState::null) {
				tmp->graphNode()->top()->state(NodeState::dirty);
				addNode(tmp->top());
			}
			tmp = tmp->right();
		}
		break;
	case Direction::DOWN:
		tmp = m_currentPosition->seekGraph(7, -7);
		for (i = 0; i < 15; i++) {
			tmp->right(tmp->graphNode()->rgt());
			if (tmp->graphNode()->rgt()->type() == NodeType::floor && tmp->graphNode()->rgt()->state() == NodeState::null) {
				tmp->graphNode()->rgt()->state(NodeState::dirty);
				addNode(tmp->right());
			}
			tmp = tmp->bot();
		}
		break;
	case Direction::LEFT:
		tmp = m_currentPosition->seekGraph(7, 7);
		for (i = 0; i < 15; i++) {
			tmp->bot(tmp->graphNode()->bot());
			if (tmp->graphNode()->bot()->type() == NodeType::floor && tmp->graphNode()->bot()->state() == NodeState::null) {
				tmp->graphNode()->bot()->state(NodeState::dirty);
				addNode(tmp->bot());
			}
			tmp = tmp->left();
		}
		break;
	case Direction::UP:
		tmp = m_currentPosition->seekGraph(-7, 7);
		for (i = 0; i < 15; i++) {
			tmp->left(tmp->graphNode()->lft());
			if (tmp->graphNode()->lft()->type() == NodeType::floor && tmp->graphNode()->lft()->state() == NodeState::null) {
				tmp->graphNode()->lft()->state(NodeState::dirty);
				addNode(tmp->left());
			}
			tmp = tmp->top();
		}
		break;
	}
}

void Robot::getRightNodes() {
	int i;
	RobotNode* tmp;
	switch (m_direction) {
	case Direction::LEFT:
		tmp = m_currentPosition->seekGraph(-7, -7);
		for (i = 0; i < 15; i++) {
			tmp->top(tmp->graphNode()->top());
			if (tmp->graphNode()->top()->type() == NodeType::floor && tmp->graphNode()->top()->state() == NodeState::null) {
				tmp->graphNode()->top()->state(NodeState::dirty);
				addNode(tmp->top());
			}
			tmp = tmp->right();
		}
		break;
	case Direction::UP:
		tmp = m_currentPosition->seekGraph(7, -7);
		for (i = 0; i < 15; i++) {
			tmp->right(tmp->graphNode()->rgt());
			if (tmp->graphNode()->rgt()->type() == NodeType::floor && tmp->graphNode()->rgt()->state() == NodeState::null) {
				tmp->graphNode()->rgt()->state(NodeState::dirty);
				addNode(tmp->right());
			}
			tmp = tmp->bot();
		}
		break;
	case Direction::RIGHT:
		tmp = m_currentPosition->seekGraph(7, 7);
		for (i = 0; i < 15; i++) {
			tmp->bot(tmp->graphNode()->bot());
			if (tmp->graphNode()->bot()->type() == NodeType::floor && tmp->graphNode()->bot()->state() == NodeState::null) {
				tmp->graphNode()->bot()->state(NodeState::dirty);
				addNode(tmp->bot());
			}
			tmp = tmp->left();
		}
		break;
	case Direction::DOWN:
		tmp = m_currentPosition->seekGraph(-7, 7);
		for (i = 0; i < 15; i++) {
			tmp->left(tmp->graphNode()->lft());
			if (tmp->graphNode()->lft()->type() == NodeType::floor && tmp->graphNode()->lft()->state() == NodeState::null) {
				tmp->graphNode()->lft()->state(NodeState::dirty);
				addNode(tmp->left());
			}
			tmp = tmp->top();
		}
		break;
	}
}


//Nodes stack management
void Robot::addNode(RobotNode* p_graphNode) {
	m_targetNodesStack->push_back(p_graphNode);
}

void Robot::addNode(RobotNode* p_graphNode, int p_index) {
	if (p_graphNode)
		m_targetNodesStack->insert(m_targetNodesStack->begin() + p_index, p_graphNode);
}

int Robot::stackLength() {
	return m_targetNodesStack->size();
}

void Robot::evaluateStack() {
	int length = stackLength();
	int i;

	for (i = 0; i < length; i++) {
		m_targetNodesStack->at(i)->calculateEvaluation(m_currentPosition, m_direction);
	}
}

void Robot::sortStack() {
	std::sort(m_targetNodesStack->begin(), m_targetNodesStack->end(), sortFunction);
}

void Robot::removeNode(RobotNode* p_graphNode) {
	if (!p_graphNode) return;

	std::vector<RobotNode*>::iterator it;
	it = std::find(m_targetNodesStack->begin(), m_targetNodesStack->end(), p_graphNode);

	if (it != m_targetNodesStack->end())
		m_targetNodesStack->erase(it);
}

void Robot::removeNode(int p_index) {
	m_targetNodesStack->erase(m_targetNodesStack->begin() + p_index);
}

void Robot::clearStack() {
	m_targetNodesStack->clear();
}


//Zone stack management
void Robot::getZone() {
	//Loop variables
	int i, j;

	clearZoneStack();

	RobotNode* target_node = m_targetNodesStack->at(0);
	
	for (i = 0; i < 15; i++) {
		for (j = 0; j < 15; j++) {
			m_targetZoneStack->push_back(target_node->seekGraph(i - 7, j - 7));
		}
	}

	//Removing NULL nodes
	int stackLength = m_targetZoneStack->size();
	for (i = 0; i < stackLength; i++) {
		if (m_targetZoneStack->at(i)->graphNode() == NULL) {
			m_targetZoneStack->erase(m_targetZoneStack->begin() + i);
			stackLength = m_targetZoneStack->size();
			i--;
		}
	}
}

void Robot::evaluateZoneStack() {
	//Loop variable
	int i;
	
	int stack_length = m_targetZoneStack->size();
	
	for (i = 0; i < stack_length; i++) {
		m_targetZoneStack->at(i)->calculateEvaluation(m_currentPosition, m_direction);
	}
}

void Robot::sortZoneStack() {
	std::sort(m_targetZoneStack->begin(), m_targetZoneStack->end(), sortFunction);
}

RobotNode* Robot::getTargetNode() {
	//Loop variable
	int i;

	int stack_length = m_targetZoneStack->size();
	int current_threshhold = INT_MAX;

	RobotNode* target_node = NULL;

	for (i = 0; i < stack_length; i++) {
		if (m_targetZoneStack->at(i)->evaluation() < current_threshhold && m_targetZoneStack->at(i)->evaluation() >= 0) {
			current_threshhold = m_targetZoneStack->at(i)->evaluation();
			target_node = m_targetZoneStack->at(i);
		}
	}

	return target_node;
}

void Robot::clearZoneStack() {
	m_targetZoneStack->clear();
}


//A* algorithm
void Robot::a_star() {
	std::vector<RobotNode*> closedList;
	std::vector<RobotNode*> openedList;

	RobotNode* u = NULL;
	RobotNode* targetNode = NULL;
	
	std::vector<RobotNode*>::iterator closedListReference;
	std::vector<RobotNode*>::iterator openedListReference;

	while (!m_targetZoneStack->empty()) {
		//Clearing lists
		openedList.clear();
		closedList.clear();

		//Extracting best node from the stack
		targetNode = *(m_targetZoneStack->end() - 1);
		m_targetZoneStack->erase(m_targetZoneStack->end() - 1);

		//Adding the starting point
		openedList.push_back(m_currentPosition);

		//Main loop
		while (!openedList.empty()) {
			u = openedList[0];
			openedList.erase(openedList.begin());

			if (u == targetNode) {
				m_direction = buildPath(&closedList, targetNode);
				std::cout << static_cast<int>(m_direction) << std::endl;
				forward(false);
				return;
			}
			
			if (u->top() && u->top()->graphNode() && u->top()->graphNode()->type() == NodeType::floor) {
				try {
				closedListReference = std::find(closedList.begin(), closedList.end(), u->top());
				openedListReference = std::find(openedList.begin(), openedList.end(), u->top());

				if ((closedListReference == closedList.end() && openedListReference == openedList.end()) || (*openedListReference)->cost() > u->top()->cost()) {
					u->top()->cost(u->cost() + 1 + turn(Direction::UP));
					u->top()->calculateHeuristic(targetNode);
					openedList.push_back(u->top());
				}

				}
				catch (...) {

				}
			}
			
			if (u->right() && u->right()->graphNode() && u->right()->graphNode()->type() == NodeType::floor) {
				try {
					closedListReference = std::find(closedList.begin(), closedList.end(), u->right());
					openedListReference = std::find(openedList.begin(), openedList.end(), u->right());

					if ((closedListReference == closedList.end() && openedListReference == openedList.end()) || (*openedListReference)->cost() > u->right()->cost()) {
						u->right()->cost(u->cost() + 1 + turn(Direction::RIGHT));
						u->right()->calculateHeuristic(targetNode);
						openedList.push_back(u->right());
					}
				}
				catch (...) {

				}
			}

			if (u->bot() && u->bot()->graphNode() && u->bot()->graphNode()->type() == NodeType::floor) {
				try {
					closedListReference = std::find(closedList.begin(), closedList.end(), u->bot());
					openedListReference = std::find(openedList.begin(), openedList.end(), u->bot());

					if ((closedListReference == closedList.end() && openedListReference == openedList.end()) || (*openedListReference)->cost() > u->bot()->cost()) {
						u->bot()->cost(u->cost() + 1 + turn(Direction::DOWN));
						u->bot()->calculateHeuristic(targetNode);
						openedList.push_back(u->bot());
					}
				}
				catch (...) {

				}
			}

			if (u->left() && u->left()->graphNode() && u->left()->graphNode()->type() == NodeType::floor) {
				try {
					closedListReference = std::find(closedList.begin(), closedList.end(), u->left());
					openedListReference = std::find(openedList.begin(), openedList.end(), u->left());

					if ((closedListReference == closedList.end() && openedListReference == openedList.end()) || (*openedListReference)->cost() > u->left()->cost()) {
						u->left()->cost(u->cost() + 1 + turn(Direction::LEFT));
						u->left()->calculateHeuristic(targetNode);
						openedList.push_back(u->left());
					}
				}
				catch (...) {

				}
			}

			closedList.push_back(u);
		}
	}
}

Direction Robot::buildPath(std::vector<RobotNode*>* p_closedList, RobotNode* p_targetPosition) {
	int i;
	
	while (std::find(p_closedList->begin(), p_closedList->end(), p_targetPosition) != p_closedList->end()) {
		for (i = 0; i < p_closedList->size(); i++){
			if (p_closedList->at(i)->cost() == p_targetPosition->cost() - 1) {
				p_targetPosition = p_closedList->at(i);
				if (p_targetPosition == m_currentPosition->top()) return Direction::UP;
				else if (p_targetPosition == m_currentPosition->right()) return Direction::RIGHT;
				else if (p_targetPosition == m_currentPosition->bot()) return Direction::DOWN;
				else if (p_targetPosition == m_currentPosition->left()) return Direction::LEFT;
				break;
			}
		}
	}
}

//Destroyer
Robot::~Robot() {
	delete m_targetNodesStack;
	delete m_targetZoneStack;

	int i, j;
	RobotNode* tmp = m_graph;
	RobotNode* tmp2 = m_graph->bot();
	RobotNode* tmp3 = tmp->right();
	
	for (i = 0; i < 1000; i++) {
		tmp3 = tmp->right();
		for (j = 0; j < 700; j++) {
			tmp2 = tmp->bot();
			delete tmp;
			tmp = tmp2;
		}
		tmp = tmp3;
	}
}