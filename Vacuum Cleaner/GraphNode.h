#pragma once
#define WIDTH_DRAW_AREA 1000
#define HEIGHT_DRAW_AREA 620

#include <cstdio>
#include <cstdlib>

#include "View.h"

enum class NodeType { null, floor, obstacle };
enum class NodeState { null, dirty, cleaned };
enum class NodePosition { TOP, RIGHT, BOTTOM, LEFT };

class GraphNode {
private:
	NodeType	m_type;
	NodeState	m_state;
	GraphNode*  m_top;
	GraphNode*  m_rgt;
	GraphNode*  m_bot;
	GraphNode*  m_lft;
	int         m_x;
	int         m_y;

	//Constructors
	GraphNode();
public:
	GraphNode(NodeType p_type, NodeState p_state, GraphNode* p_top, GraphNode* p_rgt, GraphNode* p_bot, GraphNode* p_lft, int x, int y);

	//Setters
	void type(NodeType p_type);
	void state(NodeState p_state);

	void top(GraphNode* p_node);
	void rgt(GraphNode* p_node);
	void bot(GraphNode* p_node);
	void lft(GraphNode* p_node);

	void adj(GraphNode* p_node, NodePosition p_position);
	void adj(GraphNode* p_node, int p_position);


	//Getters
	NodeType type();
	NodeState state();

	GraphNode* top();
	GraphNode* rgt();
	GraphNode* bot();
	GraphNode* lft();

	GraphNode* adj(NodePosition p_position);
	GraphNode* adj(int p_position);

	int x();
	int y();

	//Graph management
	GraphNode* seekGraph(int p_xPos, int p_yPos);

	// Destroyers
	~GraphNode();
};