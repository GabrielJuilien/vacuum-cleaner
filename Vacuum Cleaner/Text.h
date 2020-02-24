#pragma once
#include <string>
#include "utils.h"

enum class eFontStyle { BOLD, ITALIC, UNDERLINE, STRIKETHROUGH, NORMAL };
enum class eFontRender { SOLID, BLENDED };

class Text {
private:
	std::string m_text;
	SDL_Renderer* m_renderer;
	eFontStyle m_fontStyle;
	eFontRender m_fontRender;
	int m_fontSize;
	SDL_Texture* m_textTexture;
	SDL_Rect* m_destination;

	//Default constructor
	Text();
public:
	//Constructors
	Text(std::string p_text, SDL_Renderer* p_renderer, int p_fontSize, int x_pos, int y_pos);
	Text(std::string p_text, SDL_Renderer* p_renderer, eFontStyle p_fontStyle, eFontRender p_fontRender, int p_fontSize, int x_pos, int y_pos);

	//Setters
	void text(std::string p_text);
	void renderer(SDL_Renderer* p_renderer);
	void fontStyle(eFontStyle p_fontStyle);
	void fontRender(eFontRender p_fontRender);
	void fontSize(int p_fontSize);
	void destination(SDL_Rect* p_destination);
	void destination(int x_pos, int y_pos);

	//Getters
	std::string text(void);
	SDL_Renderer* renderer(void);
	eFontStyle fontStyle(void);
	eFontRender fontRender(void);
	int fontSize(void);
	SDL_Texture* textTexture(void);
	SDL_Rect* destination(void);
	int x_pos(void);
	int y_pos(void);
	int x_size(void);
	int y_size(void);

	//Destroyers
	~Text();
};