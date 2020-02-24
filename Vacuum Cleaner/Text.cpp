#include "Text.h"

Text::Text() {
	m_renderer = NULL;
	m_textTexture = NULL;
	m_textTexture = NULL;
}

//Constructors
Text::Text(std::string p_text, SDL_Renderer* p_renderer, int p_fontSize, int x_pos, int y_pos) {
	m_text = p_text;
	m_renderer = p_renderer;

	m_fontStyle = eFontStyle::NORMAL;
	m_fontRender = eFontRender::SOLID;
	m_fontSize = p_fontSize;

	m_destination = new SDL_Rect();
	m_destination->x = x_pos;
	m_destination->y = y_pos;

	if (p_text.length()) {
		m_text = p_text;
		TTF_Font* trebuchet = TTF_OpenFont("ressources/trebuch.ttf", 16);
		if (!trebuchet) throw TTF_GetError();
		SDL_Surface* textSurface = TTF_RenderText_Solid(trebuchet, m_text.c_str(), );

		
	}
}

Text::Text(std::string p_text, SDL_Renderer* p_renderer, eFontStyle p_fontStyle, int p_fontSize, int x_pos, int y_pos) {

}

Text::Text(std::string p_text, SDL_Renderer* p_renderer, eFontStyle p_fontStyle, eFontRender p_fontRender, int p_fontSize, int x_pos, int y_pos) {

}

//Setters
void Text::text(std::string p_text) {

}

void Text::renderer(SDL_Renderer* p_renderer) {

}

void Text::fontStyle(eFontStyle p_fontStyle) {

}

void Text::fontRender(eFontRender p_fontRender) {

}

void Text::fontSize(int p_fontSize) {

}

void Text::destination(SDL_Rect* p_destination) {

}

void Text::destination(int x_pos, int y_pos) {

}


//Getters
std::string Text::text(void) {

}

SDL_Renderer* Text::renderer(void) {

}

eFontStyle Text::fontStyle(void) {

}

eFontRender Text::fontRender(void) {

}

int Text::fontSize(void) {

}

SDL_Texture* Text::textTexture(void) {

}

SDL_Rect* Text::destination(void) {

}

int Text::x_pos(void) {

}

int Text::y_pos(void) {

}

int Text::x_size(void) {

}

int Text::y_size(void) {

}

//Destroyers
Text::~Text() {

}