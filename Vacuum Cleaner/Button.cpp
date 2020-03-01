#include "Button.h"

//Constructors
Button::Button() {
	m_icon = NULL;
	m_renderer = NULL;
	m_callbackFunction = NULL;
	m_destination = NULL;
	m_text = NULL;
}

Button::Button(SDL_Renderer* p_renderer, std::string p_iconPath, std::string p_text, int p_fontSize, SDL_Color p_textColor, SDL_Rect* p_destination, callbackFunction p_callbackFunction) {
	m_iconPath = p_iconPath;
	m_renderer = p_renderer;
	if (m_renderer && m_iconPath.length()) { //If a path was specified
		SDL_Surface* iconSurface = SDL_LoadBMP(p_iconPath.c_str());
		if (!iconSurface) { //If an error occured
			throw SDL_GetError();
		}
		m_icon = SDL_CreateTextureFromSurface(m_renderer, iconSurface);
		SDL_FreeSurface(iconSurface);
		if (!m_icon) {
			throw SDL_GetError();
		}
	}
	
	m_destination = p_destination;
	
	try {
		m_text = new Text(p_text, m_renderer, p_textColor, p_fontSize, p_destination->x + 50, p_destination->y + p_destination->h / 2 - p_fontSize / 2);
	}
	catch(const char* exception) {
		throw exception;
	}
	
	m_callbackFunction = p_callbackFunction;
}

//Setters
void Button::icon(std::string p_iconPath) {
	if (!p_iconPath.length()) {
		SDL_DestroyTexture(m_icon);
		m_icon = NULL;
		m_iconPath.clear();
		return;
	}
	else {
		SDL_Surface* iconSurface = SDL_LoadBMP(p_iconPath.c_str());
		if (!iconSurface) throw SDL_GetError();
		m_icon = SDL_CreateTextureFromSurface(m_renderer, iconSurface);
		SDL_FreeSurface(iconSurface);
		if (!m_icon) {
			throw SDL_GetError();
		}
	}
}

void Button::renderer(SDL_Renderer* p_renderer) {
	m_renderer = p_renderer;
	if (m_renderer) {
		icon(m_iconPath);
		m_text->renderer(p_renderer);
	}
	
}

void Button::text(std::string p_text) {
	if (!p_text.length()) {
		delete m_text;
		m_text = NULL;
		return;
	}
	else {
		try {
			m_text = new Text(p_text, m_text->renderer(), m_text->fontColor(), m_text->fontSize(), m_text->destination()->x + 50, m_text->destination()->y + m_text->destination()->h / 2 - m_text->destination()->y / 2);
		}
		catch (const char* exception) {
			throw exception;
		}
	}
}

void Button::setCallbackFunction(callbackFunction p_callbackFunction) {
	m_callbackFunction = p_callbackFunction;
}


//Getters
std::string Button::iconPath(void) {
	return m_iconPath;
}

SDL_Renderer* Button::renderer(void) {
	return m_renderer;
}

SDL_Texture* Button::iconTexture(void) {
	return m_icon;
}

callbackFunction Button::getCallbackFunction(void) {
	return m_callbackFunction;
}

//Event management
bool Button::hover(int x_mousePos, int y_mousePos) {
	if (x_mousePos > m_destination->x&& x_mousePos < m_destination->x + m_destination->w && y_mousePos > m_destination->y&& y_mousePos < m_destination->y + m_destination->h) {
		return true;
	}
	else return false;
}

bool Button::trigger(int x_mousePos, int y_mousePos, void* p_input) {
	if (hover(x_mousePos, y_mousePos)) {
		try {
			m_callbackFunction(p_input);
			return true;
		}
		catch (std::string exception) {
			throw exception;
		}
	}
	else
		return false;
}


//Destroyers
Button::~Button() {
	SDL_DestroyTexture(m_icon);
	m_icon = NULL;

	delete m_text;
	m_text = NULL;

	delete m_destination;
	m_destination = NULL;
	
	m_callbackFunction = NULL;
}