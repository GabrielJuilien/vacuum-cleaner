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
	std::vector<Rect*>* drawing;

	//Constructors
	View();
public:
	View(Rect m_xScale, Rect m_yScale, Rect m_viewer);


	//Display management
	void render(SDL_Renderer* p_renderer, int p_xParentPos, int p_yParentPos);

	//Destoyer
	~View();
};