#pragma once
#include "utils.h"
#include <string>

typedef int (*callbackFunction)(void* input);

class Button {
protected:
	std::string m_iconPath;
	SDL_Texture* m_icon;

	std::string m_text;
	SDL_Texture* m_textTexture;
	
	SDL_Rect* m_destination;

	int (*m_callbackFunction)(void* input);

public:

	//Constructors
	Button();
	Button(SDL_Renderer* p_renderer, std::string p_iconPath, std::string p_text, int p_fontSize, SDL_Color p_textColor, SDL_Rect* p_destination, callbackFunction p_callbackFunction);

	//Setters
	void icon(std::string p_iconPath);
	void text(std::string p_text);
	void setCallbackFunction(callbackFunction p_callbackFunction);

	//Getters
	std::string iconPath(void);
	SDL_Texture* iconTexture(void);
	std::string text(void);
	SDL_Texture* textTexture(void);
	callbackFunction getCallbackFunction(void);

	//Destroyers
	Button();
};
