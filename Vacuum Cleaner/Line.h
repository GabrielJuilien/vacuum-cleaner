#pragma once
#include "utils.h"

#define SOLID true;
#define DASHED false;

class Line {
private:
	int m_x1, m_y1;
	int m_x2, m_y2;
	SDL_Color m_color;
	bool m_style;

	//Constructors
	Line();
public:
	Line(int p_x1, int p_y1, int p_x2, int p_y2);
	Line(int p_x1, int p_y1, int p_x2, int p_y2, SDL_Color p_color, bool p_style);

	//Setters
	void x1(int p_x1);
	void y1(int p_y1);
	void origin(int p_x1, int p_y1);
	void origin(SDL_Point p_origin);
	void x2(int p_x2);
	void y2(int p_y2);
	void target(int p_x2, int p_y2);
	void target(SDL_Point p_target);
	void coord(int p_x1, int p_y1, int p_x2, int p_y2);
	void style(bool p_style);

	//Getters
	int x1();
	int y1();
	SDL_Point origin();
	int x2();
	int y2();
	SDL_Point target();
	bool style();

	//Display management
	void render(SDL_Renderer* p_renderer, int x_parentPos, int y_parentPos);

	//Destroyer
	~Line();
};