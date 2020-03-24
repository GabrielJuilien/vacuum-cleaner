#include "Line.h"

Line::Line() {
	m_x1 = 0;
	m_y1 = 0;
	m_x2 = 0;
	m_y2 = 0;
	m_color = { 0, 0, 0, 0 };
	m_style = SOLID;
}

Line::Line(int p_x1, int p_y1, int p_x2, int p_y2) {
	m_x1 = p_x1;
	m_y1 = p_y1;
	m_x2 = p_x2;
	m_y2 = p_x2;
	m_color = { 0, 0, 0, 0 };
	m_style = SOLID;
}

Line::Line(int p_x1, int p_y1, int p_x2, int p_y2, SDL_Color p_color, bool p_style) {
	m_x1 = p_x1;
	m_y1 = p_y1;
	m_x2 = p_x2;
	m_y2 = p_x2;
	m_color = p_color;
	m_style = p_style;
}


//Setters
void Line::x1(int p_x1) {
	m_x1 = p_x1;
}

void Line::y1(int p_y1) {
	m_y1 = p_y1;
}

void Line::origin(int p_x1, int p_y1) {
	m_x1 = p_x1;
	m_y1 = p_y1;
}

void Line::origin(SDL_Point p_origin) {
	m_x1 = p_origin.x;
	m_y1 = p_origin.y;
}

void Line::x2(int p_x2) {
	m_x2 = p_x2;
}

void Line::y2(int p_y2) {
	m_y2 = p_y2;
}

void Line::target(int p_x2, int p_y2) {
	m_x2 = p_x2;
	m_y2 = p_y2;
}

void Line::target(SDL_Point p_target) {
	m_x2 = p_target.x;
	m_y2 = p_target.y;
}

void Line::coord(int p_x1, int p_y1, int p_x2, int p_y2) {
	m_x1 = p_x1;
	m_y1 = p_y1;
	m_x2 = p_x2;
	m_y2 = p_y2;
}

void Line::style(bool p_style) {
	m_style = p_style;
}


//Getters
int Line::x1() {
	return m_x1;
}

int Line::y1() {
	return m_y1;
}

SDL_Point Line::origin(){
	return { m_x1, m_y1 };
}

int Line::x2() {
	return m_x2;
}

int Line::y2() {
	return m_y2;
}

SDL_Point Line::target() {
	return { m_x2, m_y2 };
}

bool Line::style() {
	return m_style;
}


//Display management
void Line::render(SDL_Renderer* p_renderer, int x_parentPos, int y_parentPos) {
	SDL_SetRenderDrawColor(p_renderer, m_color.r, m_color.g, m_color.b, m_color.a);
	if (m_style) {
		SDL_RenderDrawLine(p_renderer, m_x1, m_y1, m_x2, m_y2);
	}
	else {
		//Getting distances
		float i, j = m_x1, k = m_y1;
		float x = abs(m_x1 - m_x2);
		float y = abs(m_y1 - m_y2);
		float z = sqrt(pow(x, 2) + pow(y, 2));
		float x_step = x / z;
		float y_step = y / z;

		SDL_SetRenderDrawColor(p_renderer, m_color.r, m_color.g, m_color.b, m_color.a);

		for (int i = 0; i < z; i++) {
			if (x_step > 0) {
				if ((int)j % 10 < 6)
					SDL_RenderDrawPoint(p_renderer, j + x_parentPos, k + y_parentPos);
			}
			else {
				if ((int)k % 10 < 6) {
					SDL_RenderDrawPoint(p_renderer, j + x_parentPos, k + y_parentPos);
				}
			}
			
			j += x_step;
			k += y_step;
		}
	}
	SDL_SetRenderDrawColor(p_renderer,  255, 255, 255, 0 );
}

Line::~Line() {

}