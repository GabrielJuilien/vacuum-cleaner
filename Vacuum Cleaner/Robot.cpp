#include "Robot.h"

//Constructors
Robot::Robot() {
	m_stack = new std::vector<GraphNode*>();
	m_currentPosition = NULL;
	m_graph = NULL;
}

Robot::Robot(GraphNode* p_currentPosition, int p_width, int p_height) {
	m_stack = new std::vector<GraphNode*>();
	m_currentPosition = p_currentPosition;
	
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
}

//Setters
void Robot::clearStack() {
	m_stack->clear();
}