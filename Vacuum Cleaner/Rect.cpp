#include "Rect.h"

Rect::Rect() {
	m_x = 0;
	m_y = 0;
	m_w = 0;
	m_h = 0;
	m_draw = true;
}

Rect::Rect(int p_x, int p_y, int p_w, int p_h, bool p_draw) {
	m_x = p_x;
	m_y = p_y;
	m_w = p_w;
	m_h = p_h;
	m_draw = p_draw;
}

void Rect::origin(SDL_Point p) {
	/*if (p.x < 0) p.x = 0;
	if (p.y < 0) p.y = 0;*/

	m_x = p.x;
	m_y = p.y;
}

void Rect::target(SDL_Point p) {
	/*if (p.x < 0) p.x = 0;
	if (p.y < 0) p.y = 0;*/
	
	m_w = p.x - m_x;
	m_h = p.y - m_y;
	
	/*if (m_w < 0) {
		m_x += m_w;
		m_w *= -1;
	}
	if (m_h < 0) {
		m_y += m_h;
		m_h *= -1;
	}*/
}

void Rect::x(int p_x) {
	/*m_x = (p_x < 0) ? 0 : p_x;*/
	m_x = p_x;
}

void Rect::y(int p_y) {
	/*m_y = (p_y < 0) ? 0 : p_y;*/
	m_y = p_y;
}

void Rect::w(int p_w) {
	/*m_w = (p_w < 0) ? 0 : p_w;*/
	m_w = p_w;
}

void Rect::h(int p_h) {
	/*m_h = (p_h < 0) ? 0 : p_h;*/
	m_h = p_h;
}


//Getters
SDL_Point Rect::origin() {
	return { m_x, m_y };
}

SDL_Point Rect::target() {
	return { m_x + m_w, m_h + m_y };
}

bool Rect::draw() {
	return m_draw;
}

int Rect::x() {
	return m_x;
}

int Rect::y() {
	return m_y;
}

int Rect::w() {
	return m_w;
}

int Rect::h() {
	return m_h;
}


//Display management
void Rect::render(SDL_Renderer* p_renderer, SDL_Color p_color) {
	SDL_Rect tmp = { m_x, m_y, m_w, m_h };
	SDL_SetRenderDrawColor(p_renderer, p_color.r, p_color.g, p_color.b, p_color.a);
	SDL_RenderFillRect(p_renderer, &tmp);
	SDL_SetRenderDrawColor(p_renderer, 255, 255, 255, 0);
}

void Rect::render(SDL_Renderer* p_renderer, SDL_Color p_color, vec2 p_viewCenter, int p_xParentPos, int p_yParentPos, float p_zoom) {
	SDL_SetRenderDrawColor(p_renderer, p_color.r, p_color.g, p_color.b, p_color.a);
	SDL_Rect tmp = { ((float)(m_x - p_viewCenter.x) * p_zoom + 1000.0f) / PX_SIZE + p_xParentPos, (float)((m_y - p_viewCenter.y) * p_zoom + 700.0f) / PX_SIZE + p_yParentPos, m_w * p_zoom / PX_SIZE, m_h * p_zoom / PX_SIZE};
	SDL_RenderFillRect(p_renderer, &tmp);
	SDL_SetRenderDrawColor(p_renderer, 255, 255, 255, 0);
}


//Destroyers
Rect::~Rect() {

}