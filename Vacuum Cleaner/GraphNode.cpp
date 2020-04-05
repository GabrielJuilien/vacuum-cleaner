#include "GraphNode.h"


//Constructors
GraphNode::GraphNode()
{
	m_type  = NodeType::null;
	m_state = NodeState::null;
	m_top   = NULL;
	m_rgt   = NULL;
	m_bot   = NULL;
	m_lft   = NULL;
	m_x     = 0;
	m_y     = 0;
}

GraphNode::GraphNode(NodeType p_type, NodeState p_state, GraphNode* p_top, GraphNode* p_rgt, GraphNode* p_bot, GraphNode* p_lft, int x, int y)
{
	m_type  = p_type;
	m_state = p_state;
	m_top   = p_top;
	m_rgt   = p_rgt;
	m_bot   = p_bot;
	m_lft   = p_lft;
	m_x     = x;
	m_y     = y;
}

//Setters
void GraphNode::type(NodeType p_type) 
{
	m_type = p_type;
}

void GraphNode::state(NodeState p_state) {
	m_state = p_state;
}

void GraphNode::top(GraphNode* p_node) {
	m_top = p_node;
}

void GraphNode::rgt(GraphNode* p_node) {
	m_rgt = p_node;
}

void GraphNode::bot(GraphNode* p_node) {
	m_bot = p_node;
}

void GraphNode::lft(GraphNode* p_node) {
	m_lft = p_node;
}

void GraphNode::adj(GraphNode* p_node, NodePosition p_position) {
	switch (p_position)
	{
	case NodePosition::TOP:
		m_top = p_node;
		break;
	case NodePosition::RIGHT:
		m_rgt = p_node;
		break;
	case NodePosition::BOTTOM:
		m_bot = p_node;
		break;
	case NodePosition::LEFT:
		m_lft = p_node;
		break;
	default:
		break;
	}
}

void GraphNode::adj(GraphNode* p_node, int p_position) {
	switch (p_position)
	{
	case 0:
		m_top = p_node;
		break;
	case 1:
		m_rgt = p_node;
		break;
	case 2:
		m_bot = p_node;
		break;
	case 3:
		m_lft = p_node;
		break;
	default:
		break;
	}
}

// Getters
NodeType GraphNode::type() {
	return m_type;
}

NodeState GraphNode::state() {
	return m_state;
}

GraphNode* GraphNode::top() {
	return m_top;
}

GraphNode* GraphNode::rgt() {
	return m_rgt;
}

GraphNode* GraphNode::bot() {
	return m_bot;
}

GraphNode* GraphNode::lft() {
	return m_lft;
}

int GraphNode::x() {
	return m_x;
}

int GraphNode::y() {
	return m_y;
}

GraphNode* GraphNode::adj(NodePosition p_position) {
	switch (p_position)
	{
	case NodePosition::TOP:
		return m_top;
		break;
	case NodePosition::RIGHT:
		return m_rgt;
		break;
	case NodePosition::BOTTOM:
		return m_bot;
		break;
	case NodePosition::LEFT:
		return m_lft;
		break;
	default:
		return NULL;
		break;
	}
}

GraphNode* GraphNode::adj(int p_position) {
	switch (p_position)
	{
	case 0:
		return m_top;
		break;
	case 1:
		return m_rgt;
		break;
	case 2:
		return m_bot;
		break;
	case 3:
		return m_lft;
		break;
	default:
		return NULL;
		break;
	}
}

GraphNode* GraphNode::seekGraph(int p_xPos, int p_yPos) {
	//Loop variable
	int i;
	
	GraphNode* tmp = this;

	for (i = 0; i < p_xPos; i++) {
		if (tmp->rgt())
			tmp = tmp->rgt();
		else return NULL;
	}
	for (i = 0; i < p_yPos; i++) {
		if (tmp->bot())
			tmp = tmp->bot();
		else return NULL;
	}
	return tmp;
}

// Destroyer
GraphNode::~GraphNode() {

}
