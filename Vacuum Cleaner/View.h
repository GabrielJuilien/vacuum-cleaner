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
	View(SDL_Renderer* p_renderer, Rect p_xScale, Rect p_yScale, Rect p_viewer);

	//Setters
	void setXScaleBeginValue(float p_beginValue);
	void setXScaleEndValue(float p_endValue);
	void setXScaleValues(float p_beginValue, float p_endValue);

	void setYScaleBeginValue(float p_beginValue);
	void setYScaleEndValue(float p_endValue);
	void setYScaleValues(float p_beginValue, float p_endValue);

	void moveCenter(int p_xDisplacement, int p_yDisplacement);		//both arguments are in pixels
	void zoom(float p_step, int p_xMousePos, int p_yMousePos);

	//Getters
	Scale* xScale();
	Scale* yScale();
	Rect* viewer();
	float zoom();
	SDL_Point viewCenter();
	std::vector<Rect*>* drawing();

	//Display management
	void updateXScale(SDL_Renderer* p_renderer, int p_xParentPos, int p_yParentPos);
	void updateYScale(SDL_Renderer* p_renderer, int p_xParentPos, int p_yParentPos);
	void updateScales(SDL_Renderer* p_renderer, int p_xParentPos, int p_yParentPos);
	void render(SDL_Renderer* p_renderer, int p_xParentPos, int p_yParentPos);

	//Destoyer
	~View();
};