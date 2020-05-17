#include "Robot.h"

//Constructors
Robot::Robot() {
	m_targetNodeStack = new std::vector<RobotNode*>();
	m_targetZoneStack = new std::vector<RobotNode*>();
	m_currentPosition = NULL;
	m_graph = NULL;
	m_direction = Direction::UP;
}

Robot::Robot(int p_width, int p_height) {
	m_targetNodeStack = new std::vector<RobotNode*>();
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
			getBackNodes();

			int i, j;
			RobotNode* tmp = NULL;
			for (i = 0; i < 15; i++) {
				for (j = 0; j < 15; j++) {
					tmp = currentPosition()->seekGraph(i - 7, j - 7);
					tmp->graphNode()->state(NodeState::cleaned);
					removeNode(tmp);
				}
			}
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
		switch (m_direction) {
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

		if (!tmp->graphNode() || tmp->graphNode()->type() == NodeType::null) res += 1;
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
			if (!tmp || tmp->graphNode() == NULL || tmp->graphNode()->type() != NodeType::floor)
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


//Stack management
void Robot::addNode(RobotNode* p_graphNode) {
	m_targetNodeStack->push_back(p_graphNode);
}

void Robot::addNode(RobotNode* p_graphNode, int p_index) {
	if (p_graphNode)
		m_targetNodeStack->insert(m_targetNodeStack->begin() + p_index, p_graphNode);
}

bool Robot::find(RobotNode* p_node) {
	if (std::find(m_targetNodeStack->begin(), m_targetNodeStack->end(), p_node) != m_targetNodeStack->end()) return true;
	else return false;
}

int Robot::stackLength() {
	return m_targetNodeStack->size();
}

void Robot::evaluateStack() {
	int length = stackLength();
	int i;

	for (i = 0; i < length; i++) {
		m_targetNodeStack->at(i)->calculateEvaluation(m_currentPosition, m_direction);
	}
}

void Robot::sortStack() {
	std::sort(m_targetNodeStack->begin(), m_targetNodeStack->end(), sortFunction);
}

void Robot::removeNode(RobotNode* p_graphNode) {
	if (!p_graphNode) return;

	std::vector<RobotNode*>::iterator it;
	it = std::find(m_targetNodeStack->begin(), m_targetNodeStack->end(), p_graphNode);

	if (it != m_targetNodeStack->end())
		m_targetNodeStack->erase(it);
}

void Robot::removeNode(int p_index) {
	m_targetNodeStack->erase(m_targetNodeStack->begin() + p_index);
}

void Robot::clearStack() {
	m_targetNodeStack->clear();
}

bool Robot::emptyNodeStack() {
	if (m_targetNodeStack->size() == 0) return true;
	else return false;
}

//Zone stack management
void Robot::getZone(int p_endOffset) {
	//Loop variables
	int i, j, k;

	m_targetZoneStack->clear();

	RobotNode* target_node = m_targetNodeStack->at(m_targetNodeStack->size() - p_endOffset);
	RobotNode* tmp = NULL;
	bool resetLoop = false;

	for (i = 0; i < 15; i++) {
		for (j = 0; j < 15; j++) {
			if (target_node->seekGraph(i - 7, j - 7)->graphNode() != NULL && canStandOn(target_node->seekGraph(i - 7, j - 7)))
				m_targetZoneStack->push_back(target_node->seekGraph(i - 7, j - 7));
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
	return m_targetZoneStack->at(m_targetZoneStack->size() - 1);
}

void Robot::clearZoneStack() {
	m_targetZoneStack->clear();
}


//Path finding algorithm
bool Robot::a_star() {
	int i, j = 1, k = 1;
	RobotNode* tmp = NULL;

	std::vector<RobotNode*> P;
	std::vector<RobotNode*> Q;
	std::vector<RobotNode*> S;

	RobotNode* next_hop;
	RobotNode* source = NULL, *target = NULL;
	RobotNode* u = NULL, *v = NULL;

	evaluateStack();
	sortStack();

	while (j <= m_targetNodeStack->size()) {

		getZone(j);
		evaluateZoneStack();
		sortZoneStack();

		while (k <= m_targetZoneStack->size()) {
			P.clear();
			S.clear();
			Q.clear();

			source = m_currentPosition;
			target = m_targetZoneStack->at(m_targetZoneStack->size() - k);

			u = source;
			u->previous(NULL);
			P.push_back(u);

			while (P.empty() != true && u != target) {
				if (std::find(Q.begin(), Q.end(), u->top()) == Q.end() && std::find(P.begin(), P.end(), u->top()) == P.end() && canStandOn(u->top())) {
					u->top()->previous(u);
					P.push_back(u->top());
				}
				if (std::find(Q.begin(), Q.end(), u->right()) == Q.end() && std::find(P.begin(), P.end(), u->right()) == P.end() && canStandOn(u->right())) {
					u->right()->previous(u);
					P.push_back(u->right());
				}
				if (std::find(Q.begin(), Q.end(), u->bot()) == Q.end() && std::find(P.begin(), P.end(), u->bot()) == P.end() && canStandOn(u->bot())) {
					u->bot()->previous(u);
					P.push_back(u->bot());
				}
				if (std::find(Q.begin(), Q.end(), u->left()) == Q.end() && std::find(P.begin(), P.end(), u->left()) == P.end() && canStandOn(u->left())) {
					u->left()->previous(u);
					P.push_back(u->left());
				}
				Q.push_back(u);
				P.erase(std::find(P.begin(), P.end(), u));

				for (i = 0; i < P.size(); i++) {
					if (P.at(i)->previous() == P.at(i)->bot())
						P.at(i)->calculateEvaluation(target, Direction::UP);
					else if (P.at(i)->previous() == P.at(i)->left())
						P.at(i)->calculateEvaluation(target, Direction::RIGHT);
					else if (P.at(i)->previous() == P.at(i)->top())
						P.at(i)->calculateEvaluation(target, Direction::DOWN);
					else if (P.at(i)->previous() == P.at(i)->right())
						P.at(i)->calculateEvaluation(target, Direction::LEFT);
				}

				std::sort(P.begin(), P.end(), sortFunction);

				u = P.at(P.size() - 1);
			}

			if (u == target) {
				while (u) {
					S.push_back(u);
					u = u->previous();
				}

				next_hop = S.at(S.size() - 2);
				if (next_hop == m_currentPosition->top()) {
					m_direction = Direction::UP;
					forward(false);
					return true;
				}
				else if (next_hop == m_currentPosition->right()) {
					m_direction = Direction::RIGHT;
					forward(false);
					return true;
				}
				else if (next_hop == m_currentPosition->bot()) {
					m_direction = Direction::DOWN;
					forward(false);
					return true;
				}
				else if (next_hop == m_currentPosition->left()) {
					m_direction = Direction::LEFT;
					forward(false);
					return true;
				}
			}
			k++;
		}
		j++;
	}

	return false;
}

//Destroyer
Robot::~Robot() {
	delete m_targetNodeStack;

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