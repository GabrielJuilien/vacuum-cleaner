#pragma once

#include "GraphNode.h"
#include "RobotNode.h"

enum class Direction {UP, DOWN, LEFT, RIGHT};

class Robot {
private:
	std::vector<RobotNode*>* m_stack;
	RobotNode* m_currentPosition;
	RobotNode* m_graph;
public:
	//Constructors
	Robot();
	Robot(GraphNode* p_currentPosition, int p_width, int p_height);

	//Setters
	void clearStack();
	void addNode(RobotNode* p_graphNode);
	void addNode(RobotNode* p_graphNode, int p_index);
	void removeNode(RobotNode* p_graphNode);
	void removeNode(int p_index);
	
	//Getters
	RobotNode* currentPosition();

	//Movement management
	float turn(Direction p_direction); //Returns time needed to turn to p_direction
	bool forward(); //Returns false if movement is impossible
	bool backward(); //Returns false if movement is impossible

	//Captors state
	unsigned int getFrontState();
	unsigned int getBackState();

	unsigned int getFullRightState();
	unsigned int getFirstRightState();
	unsigned int getLastRightState();
	
	unsigned int getFullLeftState();
	unsigned int getFirstLeftState();
	unsigned int getLastLeftState();

	//Retrieve datas from graph
	std::vector<RobotNode*> getFrontNodes();
	std::vector<RobotNode*> getBackNodes();
	
	std::vector<RobotNode*> getLeftNodes();
	RobotNode* getFirstLeftNode();
	RobotNode* getLastLeftNode();

	std::vector<RobotNode*> getRightNodes();
	RobotNode* getFirstRightNode();
	RobotNode* getLastRightNode();

};