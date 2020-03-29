#include "View.h"

//Constructors
View::View() : Rect(0, 0, 0, 0, false) {
	m_xScale = NULL;
	m_yScale = NULL;
	m_viewer = NULL;

	m_zoom = 0;
	m_viewCenter = { 0, 0 };
	
	m_drawing = NULL;
	m_drawingBuffer = NULL;
	m_drawingTextX = NULL;
	m_drawingTextY = NULL;

	m_lineX = NULL;
	m_lineY = NULL;
}

View::View(SDL_Renderer* p_renderer, Rect p_xScale, Rect p_yScale, Rect p_viewer) : Rect(p_yScale.x(), p_xScale.y(), p_viewer.w() + p_yScale.w(), p_viewer.h() + p_xScale.h(), false) {
	//Initializing scales at default zoom
	m_xScale = new Scale(p_renderer, { p_xScale.x(), p_xScale.y() }, { p_xScale.w(), p_xScale.h() }, 0, 1000 * PX_SIZE, 50, Orientation::HORIZONTAL);
	m_yScale = new Scale(p_renderer, { p_yScale.x(), p_yScale.y() }, { p_yScale.w(), p_yScale.h() }, 0, 700 * PX_SIZE, 50, Orientation::VERTICAL);
	m_viewer = new Rect(p_viewer);

	m_zoom = ZOOM_MIN;

	//Setting view center at default position
	m_viewCenter.x = 500 * PX_SIZE;
	m_viewCenter.y = 350 * PX_SIZE;

	//Initializing an empty drawing
	m_drawing = new std::vector<Rect*>();
	m_drawingBuffer = NULL;

	//Drawing metrics
	m_drawingTextX = new Text("", p_renderer, { 0, 0, 0, 0 }, 14, 0, 0);
	m_drawingTextY = new Text("", p_renderer, { 0, 0, 0, 0 }, 14, 0, 0);
	m_lineX = new Line(0, 0, 0, 0, { 0, 0, 0, 0 }, false);
	m_lineY = new Line(0, 0, 0, 0, { 0, 0, 0, 0 }, false);
}

//Setters
void View::setXScaleBeginValue(float p_beginValue) {
	xScale()->beginValue(p_beginValue);
}

void View::setXScaleEndValue(float p_endValue) {
	xScale()->endValue(p_endValue);
}

void View::setXScaleValues(float p_beginValue, float p_endValue) {
	xScale()->values(p_beginValue, p_endValue);
}

void View::setYScaleBeginValue(float p_beginValue) {
	yScale()->beginValue(p_beginValue);
}

void View::setYScaleEndValue(float p_endValue) {
	yScale()->endValue(p_endValue);
}

void View::setYScaleValues(float p_beginValue, float p_endValue) {
	yScale()->values(p_beginValue, p_endValue);
}

void View::moveCenter(int p_xDisplacement, int p_yDisplacement) {
	float relativeXDisplacement = (float)p_xDisplacement * PX_SIZE / m_zoom;
	float domainRightBorderPosition = (float)m_viewer->w() * PX_SIZE;
	float currentRightBorderPosition = xScale()->endValue();
	float domainLeftBorderPosition = 0.0f;
	float currentLeftBorderPosition = xScale()->beginValue();

	float relativeYDisplacement = (float)p_yDisplacement * PX_SIZE / m_zoom;
	float domainBotBorderPosition = (float)m_viewer->h() * PX_SIZE;
	float currentBotBorderPosition = yScale()->endValue();
	float domainTopBorderPosition = 0.0f;
	float currentTopBorderPosition = yScale()->beginValue();
	
	if (currentLeftBorderPosition + relativeXDisplacement >= domainLeftBorderPosition && currentRightBorderPosition + relativeXDisplacement <= domainRightBorderPosition) {
		m_viewCenter.x += (int)relativeXDisplacement;
		xScale()->values(xScale()->beginValue() + relativeXDisplacement, xScale()->endValue() + relativeXDisplacement);
	}
	if (currentTopBorderPosition + relativeYDisplacement >= domainTopBorderPosition && currentBotBorderPosition + relativeYDisplacement <= domainBotBorderPosition) {
		m_viewCenter.y += (int)relativeYDisplacement;
		yScale()->values(yScale()->beginValue() + relativeYDisplacement, yScale()->endValue() + relativeYDisplacement);
	}
}

void View::zoom(float p_step, int p_xMousePos, int p_yMousePos) {
	m_zoom = (m_zoom + p_step <= ZOOM_MAX && m_zoom + p_step >= ZOOM_MIN) ? m_zoom + p_step : m_zoom;
	setXScaleValues(m_viewCenter.x - m_viewer->w() / 2 * PX_SIZE / m_zoom, m_viewCenter.x + m_viewer->w() / 2 * PX_SIZE / m_zoom);
	setYScaleValues(m_viewCenter.y - m_viewer->h() / 2 * PX_SIZE / m_zoom, m_viewCenter.y + m_viewer->h() / 2 * PX_SIZE / m_zoom);
}


void View::setBufferOrigin(int p_xPosition, int p_yPosition, bool p_drawing, int p_xParentPos, int p_yParentPos) {
	m_drawingBuffer = new Rect(0, 0, 0, 0, p_drawing);

	float relativeXPosition = ((float)(p_xPosition - p_xParentPos) * PX_SIZE - (float)m_viewCenter.x) / m_zoom + (float)m_viewCenter.x;
	float relativeYPosition = ((float)(p_yPosition - p_yParentPos) * PX_SIZE - (float)m_viewCenter.y) / m_zoom + (float)m_viewCenter.y;
	m_drawingBuffer->x(relativeXPosition);
	m_drawingBuffer->y(relativeYPosition);
}

void View::setBufferTarget(int p_xPosition, int p_yPosition, int p_xParentPos, int p_yParentPos) {
	float relativeXPosition = ((float)(p_xPosition - p_xParentPos) * PX_SIZE - (float)m_viewCenter.x) / m_zoom + (float)m_viewCenter.x;
	float relativeYPosition = ((float)(p_yPosition - p_yParentPos) * PX_SIZE - (float)m_viewCenter.y) / m_zoom + (float)m_viewCenter.y;
	m_drawingBuffer->w(relativeXPosition - m_drawingBuffer->x());
	m_drawingBuffer->h(relativeYPosition - m_drawingBuffer->y());
}

void View::discardBuffer() {
	if (m_drawingBuffer) {
		delete m_drawingBuffer;
		m_drawingBuffer = NULL;
	}
	m_drawingTextX->text("");
	m_drawingTextY->text("");
}

void View::validateBuffer() {
	m_drawing->push_back(m_drawingBuffer);
	m_drawingBuffer = NULL;
	m_drawingTextX->text("");
	m_drawingTextY->text("");
}


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

Rect* View::drawingBuffer() {
	return m_drawingBuffer;
}

//Display management
void View::updateXText(std::string p_text, int p_xParentPos, int p_yParentPos) {

}

void View::updateYText(std::string p_text, int p_xParentPos, int p_yParentPos) {

}

void View::updateXScale(SDL_Renderer* p_renderer, int p_xParentPos, int p_yParentPos) {
	m_xScale->deleteTextures();
	m_xScale->generateTextures(p_renderer, p_xParentPos + x(), p_yParentPos + y());
}

void View::updateYScale(SDL_Renderer* p_renderer, int p_xParentPos, int p_yParentPos) {
	m_yScale->deleteTextures();
	m_yScale->generateTextures(p_renderer, p_xParentPos + x(), p_yParentPos + y());
}

void View::updateScales(SDL_Renderer* p_renderer, int p_xParentPos, int p_yParentPos) {
	m_xScale->deleteTextures();
	m_xScale->generateTextures(p_renderer, p_xParentPos + x(), p_yParentPos + y());

	m_yScale->deleteTextures();
	m_yScale->generateTextures(p_renderer, p_xParentPos + x(), p_yParentPos + y());
}

void View::render(SDL_Renderer* p_renderer, int p_xParentPos, int p_yParentPos) {

	//Rendering drawing
	for (int i = 0; i < m_drawing->size(); i++) {
		if (m_drawing->at(i)->draw())
			m_drawing->at(i)->render(p_renderer, { 100, 100, 255, 0 }, m_viewCenter, x() + m_viewer->x(), y() + m_viewer->y(), m_zoom);
		else
			m_drawing->at(i)->render(p_renderer, { 255, 255, 255, 0 }, m_viewCenter, x() + m_viewer->x(), y() + m_viewer->y(), m_zoom);
	}

	//Rendering currently-drawn rectangle
	if (m_drawingBuffer) {
		if (m_drawingBuffer->draw())
			m_drawingBuffer->render(p_renderer, { 125, 125, 255, 0 }, m_viewCenter, x() + m_viewer->x(), y() + m_viewer->y(), m_zoom);
		else
			m_drawingBuffer->render(p_renderer, { 125, 125, 125, 0 }, m_viewCenter, x() + m_viewer->x(), y() + m_viewer->y(), m_zoom);
	}

	//Rendering scales
	if (m_xScale)
		m_xScale->render(p_renderer, p_xParentPos + x(), p_yParentPos + y());
	if (m_yScale)
		m_yScale->render(p_renderer, p_xParentPos + x(), p_yParentPos + y());

	//Rendering dotted lines to current position
	if (m_xScale && m_yScale) {
		int x_mousePos, y_mousePos;
		SDL_GetMouseState(&x_mousePos, &y_mousePos);
		if (x_mousePos > p_xParentPos + m_viewer->x() && x_mousePos < p_xParentPos + m_viewer->x() + m_viewer->w() && y_mousePos > p_yParentPos + m_viewer->y() && y_mousePos < p_yParentPos + m_viewer->y() + m_viewer->h()) {
			m_lineX->coord(x_mousePos, p_yParentPos + m_viewer->y(), x_mousePos, y_mousePos);
			m_lineY->coord(p_xParentPos + m_viewer->x(), y_mousePos, x_mousePos, y_mousePos);
			m_lineX->render(p_renderer, 0, 0);
			m_lineY->render(p_renderer, 0, 0);
		}
	}
}

//Destroyer
View::~View() {
	delete m_xScale;
	delete m_yScale;
	delete m_viewer;

	for (int i = 0; i < m_drawing->size(); i++) {
		delete m_drawing->at(i);
	}
	delete m_drawing;
	if (m_drawingBuffer)
		delete m_drawingBuffer;

	delete m_drawingTextX;
	delete m_drawingTextY;
	delete m_lineX;
	delete m_lineY;
}