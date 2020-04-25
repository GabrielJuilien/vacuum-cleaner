#include "RobotNode.h"

RobotNode::RobotNode() {
	m_graphNode = NULL;
	m_top = NULL;
	m_bot = NULL;
	m_left = NULL;
	m_right = NULL;
	m_evaluation = 0;
}

RobotNode::RobotNode(GraphNode* p_graphNode) {
	m_graphNode = p_graphNode;
	m_top = NULL;
	m_bot = NULL;
	m_left = NULL;
	m_right = NULL;
	m_evaluation = 0;
}

//Setters
void RobotNode::graphNode(GraphNode* p_graphNode) {
	m_graphNode = p_graphNode;
}

void RobotNode::top(RobotNode* p_graphNode) {
	m_top = p_graphNode;
}

void RobotNode::top(GraphNode* p_graphNode) {
	if (m_top)
		m_top->m_graphNode = p_graphNode;
}

void RobotNode::bot(RobotNode* p_graphNode) {
	m_bot = p_graphNode;
}

void RobotNode::bot(GraphNode* p_graphNode) {
	if (m_bot)
		m_bot->m_graphNode = p_graphNode;
}

void RobotNode::left(RobotNode* p_graphNode) {
	m_left = p_graphNode;
}

void RobotNode::left(GraphNode* p_graphNode) {
	if (m_left)
		m_left->m_graphNode = p_graphNode;
}

void RobotNode::right(RobotNode* p_graphNode) {
	m_right = p_graphNode;
}

void RobotNode::right(GraphNode* p_graphNode) {
	if (m_right)
		m_right->m_graphNode = p_graphNode;
}

//Getters
GraphNode* RobotNode::graphNode() {
	return m_graphNode;
}

RobotNode* RobotNode::top() {
	return m_top;
}

GraphNode* RobotNode::top_() {
	if (m_top)
		return m_top->m_graphNode;
	else
		return NULL;
}

RobotNode* RobotNode::bot() {
	return m_bot;
}

GraphNode* RobotNode::bot_() {
	if (m_bot)
		return m_bot->m_graphNode;
	else
		return NULL;
}

RobotNode* RobotNode::left() {
	return m_left;
}

GraphNode* RobotNode::left_() {
	if (m_left)
		return m_left->m_graphNode;
	else
		return NULL;
}

RobotNode* RobotNode::right() {
	return m_right;
}

GraphNode* RobotNode::right_() {
	if (m_right)
		return m_right->m_graphNode;
	else
		return NULL;
}

int RobotNode::evaluation() {
	return m_evaluation;
}


//Graph management
RobotNode* RobotNode::seekGraph(int i, int j) {
	RobotNode* tmp = this;
	int k;

	if (i > 0) {
		for (k = 0; k < i; k++) {
			if (!tmp) return NULL;
			tmp = tmp->m_right;
		}
	}
	else if (i < 0) {
		for (k = i; k < 0; k++) {
			if (!tmp) return NULL;
			tmp = tmp->m_left;
		}
	}

	if (j > 0) {
		for (k = 0; k < j; k++) {
			if (!tmp) return NULL;
			tmp = tmp->m_bot;
		}
	}
	else if (j < 0) {
		for (k = j; k < 0; k++) {
			if (!tmp) return NULL;
			tmp = tmp->m_top;
		}
	}

	return tmp;
}

int RobotNode::evaluate(RobotNode* p_currentPos) { //WIP, evaluates only the distance, without considering all the nodes to clean around it.
	if (m_graphNode)
		m_evaluation = abs(p_currentPos->m_graphNode->x() - m_graphNode->x()) + abs(p_currentPos->m_graphNode->y() - m_graphNode->y());
	else
		m_evaluation = -1;

	return m_evaluation;
}

bool sortFunction(RobotNode* a, RobotNode* b) {
	if (a->evaluation() > b->evaluation()) return true;
	else return false;
}

//Destroyer
RobotNode::~RobotNode() {

}