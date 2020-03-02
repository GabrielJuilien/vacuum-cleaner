#include "Button.h"

//Constructors
Button::Button() {
	m_renderer = NULL;
	m_text = NULL;
	m_icon = NULL;
	m_background = { 0 };
	m_destination = { 0 };
	m_callbackFunction = NULL;
}

Button::Button(SDL_Renderer* p_renderer, std::string p_iconPath, std::string p_text, SDL_Color p_backgroundColor, int p_fontSize, SDL_Color p_textColor, SDL_Rect p_textDestination, SDL_Rect p_iconDestination, SDL_Rect p_buttonDestination, callbackFunction p_callbackFunction) {
	m_renderer = p_renderer;

	try {
		m_icon = new Image(m_renderer, p_iconPath, p_iconDestination.x, p_iconDestination.y);
	}
	catch (const char* exception) {
		throw exception;
	}
	
	try {
		m_text = new Text(p_text, m_renderer,0, eFontRender::BLENDED, p_textColor, p_fontSize, p_textDestination.x, p_textDestination.y);
	}
	catch(const char* exception) {
		throw exception;
	}

	m_background = p_backgroundColor;

	m_destination = p_buttonDestination;
	m_callbackFunction = p_callbackFunction;
}

//Setters
void Button::icon(std::string p_iconPath) {
	if (!p_iconPath.length()) {
		delete m_icon;
		m_icon = NULL;
		return;
	}
	else {
		Image* tmp = m_icon;
		try {
			m_icon = new Image(m_renderer, p_iconPath, tmp->source(), tmp->destination());
			delete tmp;
			tmp = NULL;
		}
		catch (const char* exception) {
			throw exception;
		}
	}
}

void Button::renderer(SDL_Renderer* p_renderer) {
	m_renderer = p_renderer;
}

void Button::text(std::string p_text) {
	if (!p_text.length()) {
		delete m_text;
		m_text = NULL;
		return;
	}
	else {
		try {
			Text* tmp = m_text;
			m_text = new Text(p_text, m_text->renderer(), m_text->fontColor(), m_text->fontSize(), m_text->destination().x, m_text->destination().y);
			
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
	return m_icon->path();
}

SDL_Renderer* Button::renderer(void) {
	return m_renderer;
}

callbackFunction Button::getCallbackFunction(void) {
	return m_callbackFunction;
}


//Render management
void Button::render() {
	SDL_SetRenderDrawColor(m_renderer, m_background.r, m_background.g, m_background.b, m_background.a);
	SDL_RenderFillRect(m_renderer, &m_destination);
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 0);
	m_text->render(m_destination.x, m_destination.y);
	m_icon->render(m_destination.x, m_destination.y);
}


//Event management
bool Button::hover(int x_mousePos, int y_mousePos) {
	if (x_mousePos > m_destination.x && x_mousePos < m_destination.x + m_destination.w && y_mousePos > m_destination.y && y_mousePos < m_destination.y + m_destination.h) {
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
	delete m_icon;
	m_icon = NULL;

	delete m_text;
	m_text = NULL;
	
	m_callbackFunction = NULL;
}