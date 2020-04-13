#pragma once

#include "GraphNode.h"
#include "RobotNode.h"

#define ROBOT_LENGTH 30
#define ROBOT_WIDTH 30

enum class Direction {UP, DOWN, LEFT, RIGHT};

class Robot {
private:
	std::vector<RobotNode*>* m_stack;
	RobotNode* m_currentPosition;
	RobotNode* m_graph; 
	Direction m_direction;
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
	void currentPosition(RobotNode* p_currentPos);
	
	//Getters
	RobotNode* currentPosition();

	//Movement management
	float turn(Direction p_direction); //Returns time needed to turn to p_direction
	bool forward(bool p_simulation); //Returns false if movement is impossible
	bool backward(bool p_simulation); //Returns false if movement is impossible

	//Captors state
	unsigned short getFrontState();
	unsigned short getBackState();
	unsigned short getLeftState();
	unsigned short getRightState();

	//Retrieve data from graph
	void getFrontNodes();
	void getBackNodes();
	void getLeftNodes();
	void getRightNodes();

	//Destroyer
	~Robot();
};