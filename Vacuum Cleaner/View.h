#pragma once
#include "utils.h"
#include "Rect.h"
#include "Scale.h"
#include "Text.h"
#include "Line.h"
#include "Image.h"

#define SDL_POINT(vec2)

class View : public Rect {
private:
	Scale* m_xScale;
	Scale* m_yScale;
	Rect* m_viewer;

	float m_zoom;
	vec2 m_viewCenter;

	std::vector<Rect*>* m_drawing;
	Rect* m_drawingBuffer;

	Text* m_drawingTextX;
	Text* m_drawingTextY;
	Line* m_lineX;
	Line* m_lineY;

	Image* m_robotImage;
	vec2 m_robotPosition;

	//Constructors
	View();
public:
	View(SDL_Renderer* p_renderer, Rect p_xScale, Rect p_yScale, Rect p_viewer);

	//Setters
	void setXScaleBeginValue(float p_beginValue);
	void setXScaleEndValue(float p_endValue);
	void setXScaleValues(float p_beginValue, float p_endValue);
	void xScale(Scale* p_scale) { m_xScale = p_scale; }

	void setYScaleBeginValue(float p_beginValue);
	void setYScaleEndValue(float p_endValue);
	void setYScaleValues(float p_beginValue, float p_endValue);
	void yScale(Scale* p_scale) { m_yScale = p_scale; }

	void moveCenter(int p_xDisplacement, int p_yDisplacement);						//both arguments are in pixels
	void zoom(float p_step, int p_xMousePos, int p_yMousePos);

	void setBufferOrigin(int p_xPosition, int p_yPosition, bool p_drawing, int p_xParentPos, int p_yParentPos);			//Both arguments are in pixels
	void setBufferTarget(int p_xPosition, int p_yPosition, int p_xParentPos, int p_yParentPos);							//Both arguments are in pixels
	void discardBuffer();
	void validateBuffer();

	void setRobotPosition(int p_xPosition, int p_yPosition);

	//Getters
	Scale* xScale();
	Scale* yScale();
	Rect* viewer();
	float zoom();
	vec2 viewCenter();
	std::vector<Rect*>* drawing();
	Rect* drawingBuffer();

	SDL_Point robotPosition() {
		return { static_cast<int>(m_robotPosition.x), static_cast<int>(m_robotPosition.y) };
	}

	//Display management
	void updateXText(int p_xParentPos, int p_yParentPos);
	void updateYText(int p_xParentPos, int p_yParentPos);

	void updateXScale(SDL_Renderer* p_renderer, int p_xParentPos, int p_yParentPos);
	void updateYScale(SDL_Renderer* p_renderer, int p_xParentPos, int p_yParentPos);
	void updateScales(SDL_Renderer* p_renderer, int p_xParentPos, int p_yParentPos);

	void updateRobotImage();

	void render(SDL_Renderer* p_renderer, int p_xParentPos, int p_yParentPos);

	//Destoyer
	~View();
};