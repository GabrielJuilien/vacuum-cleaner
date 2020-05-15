#pragma once

#include "GraphNode.h"
#include "RobotNode.h"
#include <algorithm>

#define ROBOT_LENGTH 30
#define ROBOT_WIDTH 30


class Robot {
private:
	std::vector<RobotNode*>* m_targetNodeStack;
	std::vector<RobotNode*>* m_targetZoneStack;

	RobotNode* m_currentPosition;
	RobotNode* m_graph; 
	Direction m_direction;
public:
	//Constructors
	Robot();
	Robot(int p_width, int p_height);

	//Setters
	void currentPosition(RobotNode* p_currentPos);
	
	//Getters
	RobotNode* currentPosition();
	RobotNode* graph();
	Direction direction();

	//Movement management
	float turn(Direction p_direction); //Returns time needed to turn to p_direction
	bool forward(bool p_simulation); //Returns false if movement is impossible
	bool backward(bool p_simulation); //Returns false if movement is impossible

	//Captors state
	unsigned short getFrontState();
	unsigned short getBackState();
	unsigned short getLeftState();
	unsigned short getRightState();

	//Hitbox function
	bool canStandOn(RobotNode* p_position);

	//Retrieve data from graph
	void getFrontNodes();
	void getBackNodes();
	void getLeftNodes();
	void getRightNodes();

	//Stack management
	void addNode(RobotNode* p_graphNode);
	void addNode(RobotNode* p_graphNode, int p_index);
	int stackLength();
	void evaluateStack();
	void sortStack();
	void removeNode(RobotNode* p_graphNode);
	void removeNode(int p_index);
	void clearStack();
	bool emptyNodeStack();

	//Zone stack management
	void getZone();
	void evaluateZoneStack();
	void sortZoneStack();
	RobotNode* getTargetNode();
	void clearZoneStack();

	//Path finding
	bool dijkstra();

	//Destroyer
	~Robot();
};