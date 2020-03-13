#pragma once
#include "utils.h"



class Rect {
private:
	int m_x, m_y;
	int m_w, m_h;
	bool m_draw;

	//Default constructor
	Rect();
public:
	//Constructors
	Rect(int p_x, int p_y, int p_w, int p_h, bool p_draw);

	//Setters
	void origin(SDL_Point p);
	void target(SDL_Point p);
	void x(int p_x);
	void y(int p_y);
	void w(int p_w);
	void h(int p_h);

	//Getters
	SDL_Point origin();
	SDL_Point target();
	bool draw();
	int x();
	int y();
	int w();
	int h();

	//Display management
	virtual void render(SDL_Renderer* p_renderer, SDL_Color p_color);

	//Destroyer
	virtual ~Rect();
};