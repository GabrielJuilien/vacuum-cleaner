#pragma once
#include <cstdio>
#include <cstdlib>

enum class NodeType { null, floor, obstacle };
enum class NodeState { null, dirty, cleaned };
enum class NodePosition {TOP, RIGHT, BOTTOM, LEFT };

class GraphNode {
private:

	NodeType	m_type;
	NodeState	m_state;
	GraphNode*	m_top;
	GraphNode*	m_rgt;
	GraphNode*	m_bot;
	GraphNode*	m_lft;

	GraphNode();
public:
	//Constructors
	GraphNode(NodeType p_type, NodeState p_state, GraphNode* p_top, GraphNode* p_rgt, GraphNode* p_bot, GraphNode* p_lft);

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

	//Destroyer
	~GraphNode();
};