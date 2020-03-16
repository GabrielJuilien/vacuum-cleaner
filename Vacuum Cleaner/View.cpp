#include "View.h"

//Constructors
View::View() : Rect(0, 0, 0, 0, false) {
	m_xScale = NULL;
	m_yScale = NULL;

	m_viewer = NULL;
	m_zoom = 0;

	m_viewCenter = { 0, 0 };
	m_drawing = NULL;
}

View::View(Rect p_xScale, Rect p_yScale, Rect p_viewer) : Rect(p_yScale.x(), p_xScale.y(), p_viewer.w() + p_yScale.w(), p_viewer.h() + p_xScale.h(), false) {
	m_xScale = new Scale({ p_xScale.x(), p_xScale.y() }, { p_xScale.w(), p_xScale.h() }, 0, 1000 * PX_SIZE, 50, Orientation::HORIZONTAL);
	m_yScale = new Scale({ p_yScale.x(), p_yScale.y() }, { p_yScale.w(), p_yScale.h() }, 0, 720 * PX_SIZE, 50, Orientation::VERTICAL);
	m_viewer = new Rect(p_viewer);

	m_zoom = ZOOM_MIN;
	m_viewCenter.x = 500 * PX_SIZE;
	m_viewCenter.y = 350 * PX_SIZE;
	m_drawing = new std::vector<Rect*>();
}

//Setters


//Getters
Scale* View::xScale() {
	return m_xScale;
}

Scale* View::yScale() {
	return m_yScale;
}

Rect* View::viewer() {
	return m_viewer;
}

float View::zoom() {
	return m_zoom;
}

SDL_Point View::viewCenter() {
	return m_viewCenter;
}

std::vector<Rect*>* View::drawing() {
	return m_drawing;
}


//Display management
void View::render(SDL_Renderer* p_renderer, int p_xParentPos, int p_yParentPos) {

	for (int i = 0; i < m_drawing->size(); i++) {
		if (m_drawing->at(i)->draw())
			m_drawing->at(i)->render(p_renderer, { 100, 100, 255, 0 });
		else
			m_drawing->at(i)->render(p_renderer, { 255, 255, 255, 0 });
	}

	m_xScale->render(p_renderer, p_xParentPos + x(), p_yParentPos + y());
	m_yScale->render(p_renderer, p_xParentPos, p_yParentPos);
}

View::~View() {
	delete m_xScale;
	delete m_yScale;
	delete m_viewer;
	while (m_drawing->size() > 0) {
		delete m_drawing->at(0);
		m_drawing->erase(m_drawing->begin());
	}
}