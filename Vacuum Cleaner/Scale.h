#pragma once
#include "Rect.h"
#include <vector>
#include "utils.h"
enum class Orientation {
	HORIZONTAL,
	VERTICAL
};

class Scale : public Rect {
private:
	float m_beginvalue;					//First value on the scale
	float m_endValue;					//Last value on the scale
	float m_step;						//Space between two graduations
	Orientation m_orientation;

	//Constructors
	Scale();
public:
	Scale(SDL_Point p_pos, SDL_Point p_size, float p_beginvalue, float p_endValue, float p_step, Orientation p_orientation);

	//Setters
	void beginValue(float p_beginValue);
	void endValue(float p_endValue);
	void step(float p_step);
	void orientation(Orientation p_orientation);

	//Getters
	float beginValue();
	float endValue();
	float step();
	Orientation orientation();

	//Display management
	void render(SDL_Renderer* p_renderer, int p_xParentPos, int p_yParentPos);

	//Destroyer
	~Scale();
};