#pragma once
#include "Rect.h"
#include "utils.h"
#include "Scale.h"

class View : public Rect {
private:
	Scale* m_xScale;
	Scale* m_yScale;
	Rect* m_viewer;

	float m_zoom;
	SDL_Point m_viewCenter;
	std::vector<Rect*>* m_drawing;

	//Constructors
	View();
public:
	View(Rect p_xScale, Rect p_yScale, Rect p_viewer);

	//Setters
	

	//Getters
	Scale* xScale();
	Scale* yScale();
	Rect* viewer();
	float zoom();
	SDL_Point viewCenter();
	std::vector<Rect*>* drawing();

	//Display management
	void render(SDL_Renderer* p_renderer, int p_xParentPos, int p_yParentPos);

	//Destoyer
	~View();
};