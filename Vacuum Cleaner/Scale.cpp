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
	float i;
	int texture_w, texture_h;
	float buffer = 0;
	int textBuffer = 0;
	TTF_Font* trebuchet = TTF_OpenFont("ressources/trebuc.ttf", 10);
	SDL_Surface* surface = NULL;
	SDL_Texture* texture = NULL;

	SDL_SetRenderDrawColor(p_renderer, 0, 0, 0, 0);
	if (m_orientation == Orientation::HORIZONTAL) {
		SDL_RenderDrawLine(p_renderer, x() + p_xParentPos, y() + h() + p_yParentPos, x() + w() + p_xParentPos, y() + h() + p_yParentPos);

		for (i = m_beginvalue; i < m_endValue; i += (m_endValue - m_beginvalue) / (float)w()) {
			buffer += (m_endValue - m_beginvalue) / (float)w();

			if (buffer >= m_step) {
				SDL_RenderDrawLine(p_renderer, x() + p_xParentPos + i / ((m_endValue - m_beginvalue) / w()) + h(), y() + p_yParentPos + h() - 6, x() + p_xParentPos + i * w() / (m_endValue - m_beginvalue) + h(), y() + p_yParentPos + h());
				buffer = (m_endValue - m_beginvalue) / (float)w();		
				textBuffer = (textBuffer + 1) % 4;
				if (textBuffer == 0) {
					surface = TTF_RenderText_Solid(trebuchet, std::to_string((int)floor(i)).c_str(), { 0, 0, 0, 0 });
					texture = SDL_CreateTextureFromSurface(p_renderer, surface);
					SDL_QueryTexture(texture, NULL, NULL, &texture_w, &texture_h);

					SDL_Rect tmp1 = { 0, 0, texture_w, texture_h };
					SDL_Rect tmp2 = { x() + p_xParentPos + i / ((m_endValue - m_beginvalue) / w()) + h() - texture_w / 2,  y() + p_yParentPos + 1, texture_w, texture_h };

					SDL_RenderCopy(p_renderer, texture, &tmp1, &tmp2);

					SDL_FreeSurface(surface);
					SDL_DestroyTexture(texture);
				}
			}
		}
	}
	else {
		SDL_RenderDrawLine(p_renderer, x() + w() + p_xParentPos, y() + p_yParentPos, x() + w() + p_xParentPos, y() + h() + p_yParentPos);
		for (i = m_beginvalue; i < m_endValue; i += (m_endValue - m_beginvalue) / h()) {
			if (buffer > m_step) {
				SDL_RenderDrawLine(p_renderer, x() + p_xParentPos + w() - 10, y() + p_yParentPos + i * h() / (m_endValue - m_beginvalue) + w(), x() + p_xParentPos + w(), y() + p_yParentPos + i * h() / (m_endValue - m_beginvalue) + w());
				buffer = 0;
			}
			else buffer += (m_endValue - m_beginvalue) / h();
		}
	}

	SDL_SetRenderDrawColor(p_renderer, 255, 255, 255, 0);

	TTF_CloseFont(trebuchet);
}


//Destroyer
Scale::~Scale() {

}