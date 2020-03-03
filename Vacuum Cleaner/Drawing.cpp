#include "Drawing.h"

//Default constructor
DrawingNode::DrawingNode() {
	m_width = 0;
	m_height = 0;
	m_abscissa = 0;
	m_ordinate = 0;
	m_state = NodeState::BLANK;
	m_top = NULL;
	m_lft = NULL;
	m_bot = NULL;
	m_rgt = NULL;
}

//Constructors
DrawingNode::DrawingNode(int p_width, int p_height, int p_abscissa, int p_ordinate, NodeState p_state) {
	m_width = p_width;
	m_height = p_height;
	m_abscissa = p_abscissa;
	m_ordinate = p_ordinate;
	m_state = p_state;
	m_top = NULL;
	m_lft = NULL;
	m_bot = NULL;
	m_rgt = NULL;
}

DrawingNode::DrawingNode(int p_width, int p_height, int p_abscissa, int p_ordinate, NodeState p_state, DrawingNode* p_top, DrawingNode* p_lft, DrawingNode* p_bot, DrawingNode* p_rgt){
	m_width = p_width
}

DrawingNode::DrawingNode(int p_width, int p_height, NodeState p_state, DrawingNode* p_top, DrawingNode* p_lft, DrawingNode* p_bot, DrawingNode* p_rgt){ }

//Setters
void DrawingNode::width(int p_width){ }
void DrawingNode::height(int p_height){ }
void DrawingNode::abscissa(int p_abscissa){ }
void DrawingNode::ordinate(int p_ordinate){ }
void DrawingNode::state(NodeState p_state){ }
void DrawingNode::top(DrawingNode* p_top){ }
void DrawingNode::lft(DrawingNode* p_lft){ }
void DrawingNode::bot(DrawingNode* p_bot){ }
void vrgt(DrawingNode* p_rgt){ }

//Getters
int DrawingNode::width(){ }
int DrawingNode::height(){ }
int DrawingNode::abscissa(){ }
int DrawingNode::ordinate(){ }
NodeState DrawingNode::state(){ }
DrawingNode* DrawingNode::top(){ }
DrawingNode* DrawingNode::lft(){ }
DrawingNode* DrawingNode::bot(){ }
DrawingNode* DrawingNode::rgt(){ }

//Destroyers
DrawingNode::~DrawingNode(){ }