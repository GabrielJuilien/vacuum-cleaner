#pragma once
#include "utils.h"
#include "Text.h"

typedef int (*callbackFunction)(void* input);

class Button {
protected:
	std::string m_iconPath;
	SDL_Texture* m_icon;
	SDL_Renderer* m_renderer;

	Text* m_text;
	
	SDL_Rect* m_destination;

	int (*m_callbackFunction)(void* input);

public:

	//Constructors
	Button();
	Button(SDL_Renderer* p_renderer, std::string p_iconPath, std::string p_text, int p_fontSize, SDL_Color p_textColor, SDL_Rect* p_destination, callbackFunction p_callbackFunction);

	//Setters
	void icon(std::string p_iconPath);
	void renderer(SDL_Renderer* p_renderer);
	void text(std::string p_text);
	void setCallbackFunction(callbackFunction p_callbackFunction);

	//Getters
	std::string iconPath(void);
	SDL_Renderer* renderer(void);
	SDL_Texture* iconTexture(void);
	callbackFunction getCallbackFunction(void);

	//Event management
	bool hover(int x_mousePos, int y_mousePos);
	bool trigger(int x_mousePos, int y_mousePos, void* p_input);

	//Destroyers
	~Button();
};
