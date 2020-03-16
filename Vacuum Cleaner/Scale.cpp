#include "Scale.h"

//Constructors
Scale::Scale() : Rect(0, 0, 0, 0, false) {
	m_beginvalue = 0;
	m_endValue = 0;
	m_step = 0;
	m_orientation = Orientation::HORIZONTAL;
}


Scale::Scale(SDL_Point p_pos, SDL_Point p_size, float p_beginValue, float p_endValue, float p_step, Orientation p_orientation) : Rect(p_pos.x, p_pos.y, p_size.x, p_size.y, false) {
	m_beginvalue = p_beginValue;
	m_endValue = p_endValue;
	m_step = p_step;
	m_orientation = p_orientation;
}


//Setters
void Scale::beginValue(float p_beginValue) {
	m_beginvalue = (p_beginValue < 0) ? 0 : p_beginValue;
}

void Scale::endValue(float p_endValue) {
	m_endValue = (p_endValue > m_beginvalue) ? p_endValue : m_beginvalue + 0.01f;
}

void Scale::step(float p_step) {
	m_step = (p_step > 0) ? p_step : 0.01f;
}

void Scale::orientation(Orientation p_orientation) {
	m_orientation = p_orientation;
}


//Getters
float Scale::beginValue() {
	return m_beginvalue;
}

float Scale::endValue() {
	return m_endValue;
}

float Scale::step() {
	return m_step;
}

Orientation Scale::orientation() {
	return m_orientation;
}


//Render management
void Scale::render(SDL_Renderer* p_renderer, int p_xParentPos, int p_yParentPos) {

	SDL_SetRenderDrawColor(p_renderer, 0, 0, 0, 0);
	if (m_orientation == Orientation::HORIZONTAL) {
		SDL_RenderDrawLine(p_renderer, x(), y() + h() - 1, x() + w(), y() + h() - 1);
	}
	else {
		SDL_RenderDrawLine(p_renderer, x() + w() - 1, y(), x() + w() - 1, y() + h());
	}

	SDL_SetRenderDrawColor(p_renderer, 255, 255, 255, 0);
}


//Destroyer
Scale::~Scale() {

}