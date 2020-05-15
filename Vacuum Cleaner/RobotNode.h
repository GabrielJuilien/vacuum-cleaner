#pragma once
#include "GraphNode.h"

enum class Direction { UP, DOWN, LEFT, RIGHT };

class RobotNode {
private:
	GraphNode* m_graphNode;
	RobotNode *m_top, *m_bot, *m_left, *m_right;

	int m_evaluation;
	RobotNode* m_previous;

public:
	//Constructors
	RobotNode();
	RobotNode(GraphNode* p_graphNode);

	//Setters
	void graphNode(GraphNode* p_graphNode);
	void top(RobotNode* p_graphNode);
	void top(GraphNode* p_graphNode);
	void bot(RobotNode* p_graphNode);
	void bot(GraphNode* p_graphNode);
	void left(RobotNode* p_graphNode);
	void left(GraphNode* p_graphNode);
	void right(RobotNode* p_graphNode);
	void right(GraphNode* p_graphNode);
	void previous(RobotNode* p_previous);

	//Getters
	GraphNode* graphNode();
	RobotNode* top();
	GraphNode* top_();
	RobotNode* bot();
	GraphNode* bot_();
	RobotNode* left();
	GraphNode* left_();
	RobotNode* right();
	GraphNode* right_();
	int evaluation();
	RobotNode* previous();

	//Graph management
	RobotNode* seekGraph(int i, int j);
	float calculateEvaluation(RobotNode* p_currentPos, Direction p_robotDirection);

	//Destroyer
	~RobotNode();
};

bool sortFunction(RobotNode* a, RobotNode* b);