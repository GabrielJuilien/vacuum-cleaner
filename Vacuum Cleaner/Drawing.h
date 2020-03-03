#pragma once
#include "utils.h"
#include <vector>

enum class NodeState { BLANK, FLOOR, WALL, FURNITURE };

class DrawingNode {
private:
	int m_width;
	int m_height;
	int m_abscissa;
	int m_ordinate;
	NodeState m_state;
	DrawingNode* m_top;
	DrawingNode* m_lft;
	DrawingNode* m_bot;
	DrawingNode* m_rgt;
	
	//Default constructor
	DrawingNode();
public:
	//Constructors
	DrawingNode(int p_width, int p_height, int p_abscissa, int p_ordinate, NodeState p_state);
	DrawingNode(int p_width, int p_height, int p_abscissa, int p_ordinate, NodeState p_state, DrawingNode* p_top, DrawingNode* p_lft, DrawingNode* p_bot, DrawingNode* p_rgt);
	DrawingNode(int p_width, int p_height, NodeState p_state, DrawingNode* p_top, DrawingNode* p_lft, DrawingNode* p_bot, DrawingNode* p_rgt);

	//Setters
	void width(int p_width);
	void height(int p_height);
	void abscissa(int p_abscissa);
	void ordinate(int p_ordinate);
	void state(NodeState p_state);
	void top(DrawingNode* p_top);
	void lft(DrawingNode* p_lft);
	void bot(DrawingNode* p_bot);
	void rgt(DrawingNode* p_rgt);

	//Getters
	int width();
	int height();
	int abscissa();
	int ordinate();
	NodeState state();
	DrawingNode* top();
	DrawingNode* lft();
	DrawingNode* bot();
	DrawingNode* rgt();

	//Destroyers
	~DrawingNode();
};

class Drawing {
	std::vector<DrawingNode> m_nodes;
};