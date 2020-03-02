#pragma once
#include "utils.h"

class Image {
private:
	std::string m_imagePath;
	SDL_Texture* m_image;

	SDL_Renderer* m_renderer;

	SDL_Rect m_source;
	SDL_Rect m_destination;

public:
	//Constructors
	Image();
	Image(SDL_Renderer* p_renderer, std::string p_imagePath, int x_destPos, int y_destPos);
	Image(SDL_Renderer* p_renderer, std::string p_imagePath, SDL_Rect p_source, SDL_Rect p_destination);

	//Setters
	void switchImage(std::string p_imagePath);
	void source(SDL_Rect p_source);
	void sourcePosition(int x_pos, int y_pos);
	void destination(SDL_Rect p_destination);
	void destinationPosition(int x_pos, int y_pos);

	//Getters
	std::string path(void);
	SDL_Rect source(void);
	int source_x(void);
	int source_y(void);
	int source_w(void);
	int source_h(void);
	SDL_Rect destination(void);
	int destination_x(void);
	int destination_y(void);
	int destination_w(void);
	int destination_h(void);

	//Display management
	void render(int x_parentPos, int y_parentPos);
	//Destroyers
	~Image();
};