#pragma once
#include "utils.h"
#include "Text.h"
#include "Image.h"

typedef int (*callbackFunction)(void* input);

class Button {
protected:
	SDL_Renderer* m_renderer;

	Text* m_text;
	Image* m_icon;
	SDL_Color m_background;
	
	SDL_Rect m_destination;

	int (*m_callbackFunction)(void* input);

public:

	//Constructors
	Button();
	Button(SDL_Renderer* p_renderer, std::string p_iconPath, std::string p_text, SDL_Color p_backgroundColor, int p_fontSize, SDL_Color p_textColor, SDL_Rect p_textDestination, SDL_Rect p_iconDestination, SDL_Rect p_buttonDestination, callbackFunction p_callbackFunction);

	//Setters
	void icon(std::string p_iconPath);
	void renderer(SDL_Renderer* p_renderer);
	void text(std::string p_text);
	void setCallbackFunction(callbackFunction p_callbackFunction);

	//Getters
	std::string iconPath(void);
	SDL_Renderer* renderer(void);
	callbackFunction getCallbackFunction(void);

	//Render management
	void render();

	//Event management
	bool hover(int x_mousePos, int y_mousePos);
	bool trigger(int x_mousePos, int y_mousePos, void* p_input);

	//Destroyers
	~Button();
};
