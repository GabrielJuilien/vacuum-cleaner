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

DrawingNode::DrawingNode(int p_width, int p_height, int p_abscissa, int p_ordinate, NodeState p_state, DrawingNode* p_top, DrawingNode* p_lft, DrawingNode* p_bot, DrawingNode* p_rgt) {
	m_width = p_width;
	m_height = p_height;
	m_abscissa = p_abscissa;
	m_ordinate = p_ordinate;
	m_state = p_state;
	m_top = p_top;
	m_lft = p_lft;
	m_bot = p_bot;
	m_rgt = p_rgt;
}

DrawingNode::DrawingNode(int p_width, int p_height, NodeState p_state, DrawingNode* p_top, DrawingNode* p_lft, DrawingNode* p_bot, DrawingNode* p_rgt) {
	m_width = p_width;
	m_height = p_height;
	m_state = p_state;
	m_top = p_top;
	m_lft = p_lft;
	m_bot = p_bot;
	m_rgt = p_rgt;

	if (m_top == NULL && m_bot == NULL) {
		m_ordinate = 0;
	}
	else if (m_top == NULL) {
		m_ordinate = m_bot->ordinate() - m_height;
	}
	else {
		m_abscissa = m_top->ordinate() + m_height;
	}

	if (m_lft == NULL && m_rgt == NULL) {
		m_abscissa = 0;
	}
	else if (m_lft == NULL) {
		m_abscissa = m_rgt->abscissa() - m_width;
	}
	else {
		m_abscissa = m_lft->abscissa() + m_width;
	}
}


//Setters
void DrawingNode::width(int p_width){
	m_width = p_width;
}

void DrawingNode::height(int p_height){
	m_height = p_height;
}

void DrawingNode::abscissa(int p_abscissa){
	m_abscissa = p_abscissa;
}

void DrawingNode::ordinate(int p_ordinate){
	m_ordinate = p_ordinate;
}

void DrawingNode::state(NodeState p_state){
	m_state = p_state;
}

void DrawingNode::top(DrawingNode* p_top){
	m_top = p_top;
}

void DrawingNode::lft(DrawingNode* p_lft){
	m_lft = p_lft;
}

void DrawingNode::bot(DrawingNode* p_bot){
	m_bot = p_bot;
}

void DrawingNode::rgt(DrawingNode* p_rgt){
	m_rgt = p_rgt;
}

//Getters
int DrawingNode::width(){
	return m_width;
}
int DrawingNode::height(){
	return m_height;
}
int DrawingNode::abscissa(){
	return m_abscissa;
}
int DrawingNode::ordinate(){
	return m_ordinate;
}
NodeState DrawingNode::state(){
	return m_state;
}
DrawingNode* DrawingNode::top(){
	return m_top;
}
DrawingNode* DrawingNode::lft(){
	return m_lft;
}
DrawingNode* DrawingNode::bot(){
	return m_bot;
}
DrawingNode* DrawingNode::rgt(){
	return m_rgt;
}

//Destroyers
DrawingNode::~DrawingNode(){ }