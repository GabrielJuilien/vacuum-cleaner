#include "Scale.h"

//Constructors
Scale::Scale() : Rect(0, 0, 0, 0, false) {
	m_beginValue = 0;
	m_endValue = 0;
	m_step = 0;
	m_orientation = Orientation::HORIZONTAL;
	m_digits = NULL;
	m_digitsPosition = NULL;
}


Scale::Scale(SDL_Renderer* p_renderer, SDL_Point p_pos, SDL_Point p_size, float p_beginValue, float p_endValue, float p_step, Orientation p_orientation) : Rect(p_pos.x, p_pos.y, p_size.x, p_size.y, false) {
	m_beginValue = p_beginValue;
	m_endValue = p_endValue;
	m_step = p_step;
	m_orientation = p_orientation;
	m_digits = new std::vector<SDL_Texture*>();
	m_digitsPosition = new std::vector<SDL_Rect*>();
}


//Setters
void Scale::beginValue(float p_beginValue) {
	m_beginValue = (p_beginValue < 0) ? 0 : p_beginValue;
}

void Scale::values(float p_beginValue, float p_endValue) {
	m_beginValue = (p_beginValue < 0) ? 0 : p_beginValue;
	m_endValue = (p_endValue > m_beginValue) ? p_endValue : m_beginValue + 0.01f;
}

void Scale::endValue(float p_endValue) {
	m_endValue = (p_endValue > m_beginValue) ? p_endValue : m_beginValue + 0.01f;
}

void Scale::step(float p_step) {
	m_step = (p_step > 0) ? p_step : 0.01f;
}

void Scale::orientation(Orientation p_orientation) {
	m_orientation = p_orientation;
}


//Getters
float Scale::beginValue() {
	return m_beginValue;
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
void Scale::generateTextures(SDL_Renderer* p_renderer, int p_xParentPos, int p_yParentPos) {
	float i;
	int texture_w, texture_h;
	float buffer = 0;
	int textBuffer = 0;
	TTF_Font* trebuchet = TTF_OpenFont("ressources/trebuc.ttf", 10);
	SDL_Surface* surface = NULL;
	SDL_Texture* texture = NULL;

	//Creating new textures and rectangles

	if (m_orientation == Orientation::HORIZONTAL) {
		for (i = m_beginValue; i < m_endValue; i += (m_endValue - m_beginValue) / (w() - h())) {
			buffer += (m_endValue - m_beginValue) / (w() - h());
			if (buffer > m_step) {
				buffer -= m_step;
				textBuffer = (textBuffer + 1) % 4;
				if (textBuffer == 0) {
					surface = TTF_RenderText_Solid(trebuchet, std::to_string((int)ceil(i)).c_str(), { 0, 0, 0, 0 });
					texture = SDL_CreateTextureFromSurface(p_renderer, surface);
					SDL_QueryTexture(texture, NULL, NULL, &texture_w, &texture_h);

					SDL_Rect* tmp = new SDL_Rect({static_cast<int>((float)(x() + p_xParentPos) + (i - m_beginValue) * (w() - h()) / (m_endValue - m_beginValue) + h() - texture_w / 2),  static_cast<int>(y() + p_yParentPos - 1), texture_w, texture_h });

					m_digits->push_back(texture);
					m_digitsPosition->push_back(tmp);
					SDL_FreeSurface(surface);
				}
			}
		}
	}
	else {
		for (i = m_beginValue; i < m_endValue; i += (m_endValue - m_beginValue) / (h() - w())) {
			buffer += (m_endValue - m_beginValue) / (h() - w());
			if (buffer > m_step) {
				buffer -= m_step;
				textBuffer = (textBuffer + 1) % 4;
				if (textBuffer == 0) {
					surface = TTF_RenderText_Solid(trebuchet, std::to_string((int)ceil(i)).c_str(), { 0, 0, 0, 0 });
					texture = SDL_CreateTextureFromSurface(p_renderer, surface);
					SDL_QueryTexture(texture, NULL, NULL, &texture_w, &texture_h);

					SDL_Rect* tmp = new SDL_Rect({static_cast<int>(x() + p_xParentPos - 1), static_cast<int>((float)(y() + p_yParentPos) + (i - m_beginValue) * (h() - w()) / (m_endValue - m_beginValue) + w() - texture_h / 2), texture_w, texture_h });

					m_digits->push_back(texture);
					m_digitsPosition->push_back(tmp);
					SDL_FreeSurface(surface);
				}
			}
		}
	}

	TTF_CloseFont(trebuchet);
}

void Scale::deleteTextures() {
	//Delete previous textures and rectangles
	for (int i = 0; i < m_digits->size(); i++) {
		SDL_DestroyTexture(m_digits->at(i));
		delete m_digitsPosition->at(i);
	}
	m_digits->clear();
	m_digitsPosition->clear();
}

void Scale::render(SDL_Renderer* p_renderer, int p_xParentPos, int p_yParentPos) {
	float i;
	int j;
	float buffer = 0;
	SDL_Rect tmp;
	SDL_Point center;

	SDL_SetRenderDrawColor(p_renderer, 0, 0, 0, 0);
	if (m_orientation == Orientation::HORIZONTAL) {
		SDL_RenderDrawLine(p_renderer, x() + p_xParentPos, y() + h() + p_yParentPos, x() + w() + p_xParentPos, y() + h() + p_yParentPos);
		for (i = m_beginValue; i < m_endValue; i += (m_endValue - m_beginValue) / (w() - h())) {
			if (buffer >= m_step) {
				SDL_RenderDrawLine(p_renderer, (float)(x() + p_xParentPos) + (i - m_beginValue) * (w() - h()) / (m_endValue - m_beginValue) + h(), y() + p_yParentPos + h() - 6, x() + p_xParentPos + (i - m_beginValue) * (w() - h()) / (m_endValue - m_beginValue) + h(), y() + p_yParentPos + h());
				buffer -= m_step;
			}
			buffer += (m_endValue - m_beginValue) / (w() - h());
		}

	}
	else {
		SDL_RenderDrawLine(p_renderer, x() + w() + p_xParentPos, y() + p_yParentPos, x() + w() + p_xParentPos, y() + h() + p_yParentPos);
		for (i = m_beginValue; i < m_endValue; i += (m_endValue - m_beginValue) / (h() - w())) {
			if (buffer >= m_step) {
				SDL_RenderDrawLine(p_renderer, x() + p_xParentPos + w() - 6, (float)(y() + p_yParentPos) + (i - m_beginValue) * (h() - w()) / (m_endValue - m_beginValue) + w(), x() + p_xParentPos + w(), y() + p_yParentPos + (i - m_beginValue) * (h() - w()) / (m_endValue - m_beginValue) + w());
				buffer -= m_step;
			}
			buffer += (m_endValue - m_beginValue) / (h() - w());
		}
	}


	if (m_digits->empty()) generateTextures(p_renderer, p_xParentPos, p_yParentPos);
	
	for (j = 0; j < m_digits->size(); j++) {
		tmp = { 0, 0, m_digitsPosition->at(j)->w, m_digitsPosition->at(j)->h };
		center = { tmp.x + tmp.w / 2, tmp.y + tmp.h / 2 };
		if (m_orientation == Orientation::HORIZONTAL) SDL_RenderCopy(p_renderer, m_digits->at(j), &tmp, m_digitsPosition->at(j));
		else SDL_RenderCopyEx(p_renderer, m_digits->at(j), &tmp, m_digitsPosition->at(j), 270, &center, SDL_FLIP_NONE);
	}

	SDL_SetRenderDrawColor(p_renderer, 255, 255, 255, 0);

}


//Destroyer
Scale::~Scale() {
	this->deleteTextures();
	delete m_digits;
	delete m_digitsPosition;
}