#include "Robot.h"

//Constructors
Robot::Robot() {
	m_stack = new std::vector<RobotNode*>();
	m_currentPosition = NULL;
	m_graph = NULL;
	m_direction = Direction::UP;
}

Robot::Robot(int p_width, int p_height) {
	m_stack = new std::vector<RobotNode*>();
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
	if (getFrontState()) return false;
	else {
		if (!p_simulation) {
			//Moving robot to the next node
			switch (m_direction)
			{
			case Direction::UP:
				m_currentPosition = m_currentPosition->top();
				break;
			case Direction::DOWN:
				m_currentPosition = m_currentPosition->bot();
				break;
			case Direction::LEFT:
				m_currentPosition = m_currentPosition->left();
				break;
			case Direction::RIGHT:
				m_currentPosition = m_currentPosition->right();
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
			tmp = m_currentPosition->seekGraph(i - 7, 8);
			break;
		case Direction::DOWN:
			tmp = m_currentPosition->seekGraph(-i + 7, -8);
			break;
		case Direction::LEFT:
			tmp = m_currentPosition->seekGraph(-7, -i + 8);
			break;
		case Direction::RIGHT:
			tmp = m_currentPosition->seekGraph(7, i - 8);
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
			tmp = m_currentPosition->seekGraph(i - 7, -8);
			break;
		case Direction::DOWN:
			tmp = m_currentPosition->seekGraph(-i + 7, 8);
			break;
		case Direction::LEFT:
			tmp = m_currentPosition->seekGraph(7, -i + 8);
			break;
		case Direction::RIGHT:
			tmp = m_currentPosition->seekGraph(-7, i - 8);
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
			tmp = m_currentPosition->seekGraph(-8, -i + 7);
			break;
		case Direction::LEFT:
			tmp = m_currentPosition->seekGraph(-8, i - 7);
			break;
		case Direction::RIGHT:
			tmp = m_currentPosition->seekGraph(8, -i + 7);
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
			tmp = m_currentPosition->seekGraph(8, -i + 7);
			break;
		case Direction::LEFT:
			tmp = m_currentPosition->seekGraph(8, i - 7);
			break;
		case Direction::RIGHT:
			tmp = m_currentPosition->seekGraph(-8, -i + 7);
			break;
		}

		if (tmp->graphNode() && tmp->graphNode()->type() == NodeType::null) res += 1;
		res = res << 1;
	}

	return res;
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


//Stack management
void Robot::addNode(RobotNode* p_graphNode) {
	//if (p_graphNode)
		m_stack->push_back(p_graphNode);
}

void Robot::addNode(RobotNode* p_graphNode, int p_index) {
	if (p_graphNode)
		m_stack->insert(m_stack->begin() + p_index, p_graphNode);
}

int Robot::stackLength() {
	return m_stack->size();
}

void Robot::evaluateStack() {
	int length = stackLength();
	int i;

	for (i = 0; i < length; i++) {
		m_stack->at(i)->evaluate(m_currentPosition);
	}
}

void Robot::sortStack() {
	std::sort(m_stack->begin(), m_stack->end(), sortFunction);
}

void Robot::removeNode(RobotNode* p_graphNode) {
	if (!p_graphNode) return;

	std::vector<RobotNode*>::iterator it;
	it = std::find(m_stack->begin(), m_stack->end(), p_graphNode);

	if (it != m_stack->end())
		m_stack->erase(it);
}

void Robot::removeNode(int p_index) {
	m_stack->erase(m_stack->begin() + p_index);
}

void Robot::clearStack() {
	m_stack->clear();
}


//Destroyer
Robot::~Robot() {
	delete m_stack;

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