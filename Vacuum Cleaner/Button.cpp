#include "Button.h"

Button::Button() {
	m_icon = NULL;
	m_textTexture = NULL;
	m_callbackFunction = NULL;
	m_destination = NULL;
}

Button::Button(SDL_Renderer* p_renderer, std::string p_iconPath, std::string p_text, std::string fontPath, int p_fontSize, SDL_Color p_textColor, SDL_Rect* p_destination, callbackFunction p_callbackFunction) {
	m_iconPath = p_iconPath;
	if (m_iconPath.length()) { //If a path was specified
		SDL_Surface* iconSurface = SDL_LoadBMP(p_iconPath.c_str());
		if (!iconSurface) { //If an error occured
			throw SDL_GetError();
		}
		m_icon = SDL_CreateTextureFromSurface(p_renderer, iconSurface);
		SDL_FreeSurface(iconSurface);
		if (!m_icon) {
			throw SDL_GetError();
		}
	}

	m_text = p_text;
	if (p_text.length()) { //If a text was specified
		TTF_Font* font = TTF_OpenFont()
	}
}