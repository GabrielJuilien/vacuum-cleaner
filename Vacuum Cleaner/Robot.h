#pragma once

#include "GraphNode.h"

enum class Direction {UP, DOWN, LEFT, RIGHT};

class Robot {
private:
	std::vector<GraphNode*>* m_stack;
	GraphNode* m_currentPosition;
	GraphNode* m_robotGraph;
public:
	//Constructors
	Robot();
	Robot(GraphNode* p_currentPosition);

	//Setters
	void clearStack();
	void addNode(GraphNode* p_graphNode);
	void addNode(GraphNode* p_graphNode, int p_index);
	void removeNode(GraphNode* p_graphNode);
	void removeNode(int p_index);
	
	//Getters
	GraphNode* currentPosition;

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
	std::vector<GraphNode*> getFrontNodes();
	std::vector<GraphNode*> getBackNodes();
	
	std::vector<GraphNode*> getLeftNodes();
	GraphNode* getFirstLeftNode();
	GraphNode* getLastLeftNode();

	std::vector<GraphNode*> getRightNodes();
	GraphNode* getFirstRightNode();
	GraphNode* getLastRightNode();

};