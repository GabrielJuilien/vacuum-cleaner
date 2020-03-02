#include "Image.h"

//Constructors
Image::Image() {
	m_image = NULL;
	m_renderer = NULL;

	m_source = { 0 };
	m_destination = { 0 };
}

Image::Image(SDL_Renderer* p_renderer, std::string p_imagePath, int x_destPos, int y_destPos) {
	m_renderer = p_renderer;

	m_source = { 0 };
	m_destination = { 0 };

	m_imagePath = p_imagePath;
	if (m_imagePath.length()) {
		SDL_Surface* imageSurface = IMG_Load(m_imagePath.c_str());
		if (!imageSurface) {
			throw SDL_GetError();
		}

		m_image = SDL_CreateTextureFromSurface(m_renderer, imageSurface);

		if (!m_image) {
			SDL_FreeSurface(imageSurface);
			throw SDL_GetError();
		}

		SDL_FreeSurface(imageSurface);

		int w = 0, h = 0;
		SDL_QueryTexture(m_image, NULL, NULL, &w, &h);

		m_source.x = 0;
		m_source.y = 0;
		m_source.w = w;
		m_source.h = h;

		m_destination.x = x_destPos;
		m_destination.y = y_destPos;
		m_destination.w = w;
		m_destination.h = h;
	}
	else {
		m_image = NULL;
	}
}

Image::Image(SDL_Renderer* p_renderer, std::string p_imagePath, SDL_Rect p_source, SDL_Rect p_destination) {
	m_renderer = p_renderer;
	
	m_source = p_source;
	m_destination = p_destination;

	m_imagePath = p_imagePath;
	if (m_imagePath.length()) {
		SDL_Surface* imageSurface = IMG_Load(m_imagePath.c_str());
		if (!imageSurface) {
			throw SDL_GetError();
		}

		m_image = SDL_CreateTextureFromSurface(m_renderer, imageSurface);

		if (!m_image) {
			SDL_FreeSurface(imageSurface);
			throw SDL_GetError();
		}

		SDL_FreeSurface(imageSurface);
	}
	else {
		m_image = NULL;
	}
}


//Setters
void Image::switchImage(std::string p_imagePath) {
	m_imagePath = p_imagePath;
	if (m_imagePath.length()) {
		SDL_Surface* imageSurface = IMG_Load(m_imagePath.c_str());
		if (!imageSurface) {
			throw SDL_GetError();
		}

		m_image = SDL_CreateTextureFromSurface(m_renderer, imageSurface);

		if (!m_image) {
			SDL_FreeSurface(imageSurface);
			throw SDL_GetError();
		}

		SDL_FreeSurface(imageSurface);
	}
	else {
		m_image = NULL;
	}
}

void Image::source(SDL_Rect p_source) {
	m_source = p_source;
}

void Image::sourcePosition(int x_pos, int y_pos) {
	m_source.x = x_pos;
	m_source.y = y_pos;
}

void Image::destination(SDL_Rect p_destination) {
	m_destination = p_destination;
}

void Image::destinationPosition(int x_pos, int y_pos) {
	m_destination.x = x_pos;
	m_destination.y = y_pos;
}


//Getters
std::string Image::path(void) {
	return m_imagePath;
}

SDL_Rect Image::source(void) {
	return m_source;
}

int Image::source_x(void) {
	return m_source.x;
}

int Image::source_y(void) {
	return m_source.y;
}

int Image::source_w(void) {
	return m_source.w;
}

int Image::source_h(void) {
	return m_source.h;
}

SDL_Rect Image::destination(void) {
	return m_destination;
}

int Image::destination_x(void) {
	return m_destination.x;
}

int Image::destination_y(void) {
	return m_destination.y;
}

int Image::destination_w(void) {
	return m_destination.w;
}

int Image::destination_h(void) {
	return m_destination.h;
}


//Display management
void Image::render(int x_parentPos, int y_parentPos) {
	if (!m_image) return;

	SDL_Rect tmp_dest;
	tmp_dest.x = x_parentPos + m_destination.x;
	tmp_dest.y = y_parentPos + m_destination.y;
	tmp_dest.w = m_destination.w;
	tmp_dest.h = m_destination.h;

	SDL_RenderCopy(m_renderer, m_image, NULL, &tmp_dest);
}

//Destroyers
Image::~Image() {
	m_imagePath.clear();

	SDL_DestroyTexture(m_image);
	m_image = NULL;

	m_renderer = NULL;
}