#include "Text.h"

Text::Text() {
	m_renderer = NULL;
	m_fontStyle = TTF_STYLE_NORMAL;
	m_fontRender = eFontRender::SOLID;
	m_fontColor = { 0, 0, 0, 0};
	m_fontSize = 12;
	m_textTexture = NULL;
	m_destination = { 0 };
}

//Constructors
Text::Text(std::string p_text, SDL_Renderer* p_renderer, SDL_Color p_fontColor, int p_fontSize, int x_pos, int y_pos) {
	m_text = p_text;
	m_renderer = p_renderer;

	m_fontStyle = TTF_STYLE_NORMAL;
	m_fontRender = eFontRender::SOLID;
	m_fontColor = p_fontColor;
	m_fontSize = p_fontSize;

	m_destination = { 0 };
	m_destination.x = x_pos;
	m_destination.y = y_pos;

	m_text = p_text;
	if (m_text.length()) {
		TTF_Font* trebuchet = TTF_OpenFont("ressources/trebuc.ttf", 16);
		if (!trebuchet) throw TTF_GetError();

		SDL_Surface* textSurface = TTF_RenderText_Solid(trebuchet, m_text.c_str(), m_fontColor);
		if (!textSurface) {
			TTF_CloseFont(trebuchet);
			throw TTF_GetError();
		}

		m_textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
		if (!m_textTexture) {
			TTF_CloseFont(trebuchet);
			SDL_FreeSurface(textSurface);
			throw TTF_GetError();
		}

		TTF_CloseFont(trebuchet);
		SDL_FreeSurface(textSurface);
	}
	else {
		SDL_DestroyTexture(m_textTexture);
		m_textTexture = NULL;
	}

	int w, h;
	SDL_QueryTexture(m_textTexture, NULL, NULL, &w, &h);
	m_destination.w = w;
	m_destination.h = h;

}

Text::Text(std::string p_text, SDL_Renderer* p_renderer, int p_fontStyle, eFontRender p_fontRender, SDL_Color p_fontColor, int p_fontSize, int x_pos, int y_pos) {
	m_text = p_text;
	m_renderer = p_renderer;

	m_fontStyle = p_fontStyle;
	m_fontRender = p_fontRender;
	m_fontColor = p_fontColor;
	m_fontSize = p_fontSize;

	m_destination = { 0 };
	m_destination.x = x_pos;
	m_destination.y = y_pos;

	m_text = p_text;
	if (m_text.length()) {
		TTF_Font* trebuchet = TTF_OpenFont("ressources/trebuc.ttf", m_fontSize);
		if (!trebuchet) throw TTF_GetError();
		TTF_SetFontStyle(trebuchet, m_fontStyle);
		
		SDL_Surface* textSurface;
		switch (m_fontRender) {
			case eFontRender::SOLID:
				textSurface = TTF_RenderText_Solid(trebuchet, m_text.c_str(), m_fontColor);
				break;
			case eFontRender::BLENDED:
				textSurface = TTF_RenderText_Blended(trebuchet, m_text.c_str(), m_fontColor);
				break;
			default:
				textSurface = TTF_RenderText_Solid(trebuchet, m_text.c_str(), m_fontColor);
				break;
		}
		if (!textSurface) {
			TTF_CloseFont(trebuchet);
			throw TTF_GetError();
		}

		m_textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
		if (!m_textTexture) {
			TTF_CloseFont(trebuchet);
			SDL_FreeSurface(textSurface);
			throw TTF_GetError();
		}

		TTF_CloseFont(trebuchet);
		SDL_FreeSurface(textSurface);
	}
	else {
		SDL_DestroyTexture(m_textTexture);
		m_textTexture = NULL;
	}

	int w, h;
	SDL_QueryTexture(m_textTexture, NULL, NULL, &w, &h);
	m_destination.w = w;
	m_destination.h = h;
}

//Setters
void Text::text(std::string p_text) {
	if (m_text != p_text) {
		m_text = p_text;
		if (m_text.length()) {
			TTF_Font* trebuchet = TTF_OpenFont("ressources/trebuc.ttf", m_fontSize);
			if (!trebuchet) throw TTF_GetError();
			TTF_SetFontStyle(trebuchet, m_fontStyle);

			SDL_Surface* textSurface;
			switch (m_fontRender) {
			case eFontRender::SOLID:
				textSurface = TTF_RenderText_Solid(trebuchet, m_text.c_str(), m_fontColor);
				break;
			case eFontRender::BLENDED:
				textSurface = TTF_RenderText_Blended(trebuchet, m_text.c_str(), m_fontColor);
				break;
			default:
				textSurface = TTF_RenderText_Solid(trebuchet, m_text.c_str(), m_fontColor);
				break;
			}
			if (!textSurface) {
				TTF_CloseFont(trebuchet);
				throw TTF_GetError();
			}

			m_textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
			if (!m_textTexture) {
				TTF_CloseFont(trebuchet);
				SDL_FreeSurface(textSurface);
				throw TTF_GetError();
			}

			TTF_CloseFont(trebuchet);
			SDL_FreeSurface(textSurface);
		}
		else {
			SDL_DestroyTexture(m_textTexture);
			m_textTexture = NULL;
		}

		int w, h;
		SDL_QueryTexture(m_textTexture, NULL, NULL, &w, &h);
		m_destination.w = w;
		m_destination.h = h;
	}
}

void Text::renderer(SDL_Renderer* p_renderer) {
	m_renderer = p_renderer;
	if (m_renderer) {
		text(m_text);
	}
}

void Text::fontStyle(int p_fontStyle) {
	if (m_fontStyle != p_fontStyle) {
		m_fontStyle = p_fontStyle;
		if (m_text.length()) {
			TTF_Font* trebuchet = TTF_OpenFont("ressources/trebuc.ttf", m_fontSize);
			if (!trebuchet) throw TTF_GetError();
			TTF_SetFontStyle(trebuchet, m_fontStyle);

			SDL_Surface* textSurface;
			switch (m_fontRender) {
			case eFontRender::SOLID:
				textSurface = TTF_RenderText_Solid(trebuchet, m_text.c_str(), m_fontColor);
				break;
			case eFontRender::BLENDED:
				textSurface = TTF_RenderText_Blended(trebuchet, m_text.c_str(), m_fontColor);
				break;
			default:
				textSurface = TTF_RenderText_Solid(trebuchet, m_text.c_str(), m_fontColor);
				break;
			}
			if (!textSurface) {
				TTF_CloseFont(trebuchet);
				throw TTF_GetError();
			}

			m_textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
			if (!m_textTexture) {
				TTF_CloseFont(trebuchet);
				SDL_FreeSurface(textSurface);
				throw TTF_GetError();
			}

			TTF_CloseFont(trebuchet);
			SDL_FreeSurface(textSurface);
		}
		else {
			SDL_DestroyTexture(m_textTexture);
			m_textTexture = NULL;
		}

		int w, h;
		SDL_QueryTexture(m_textTexture, NULL, NULL, &w, &h);
		m_destination.w = w;
		m_destination.h = h;
	}
}

void Text::fontRender(eFontRender p_fontRender) {
	if (p_fontRender != m_fontRender) {
		m_fontRender = p_fontRender;
		if (m_text.length()) {
			TTF_Font* trebuchet = TTF_OpenFont("ressources/trebuc.ttf", m_fontSize);
			if (!trebuchet) throw TTF_GetError();
			TTF_SetFontStyle(trebuchet, m_fontStyle);

			SDL_Surface* textSurface;
			switch (m_fontRender) {
			case eFontRender::SOLID:
				textSurface = TTF_RenderText_Solid(trebuchet, m_text.c_str(), m_fontColor);
				break;
			case eFontRender::BLENDED:
				textSurface = TTF_RenderText_Blended(trebuchet, m_text.c_str(), m_fontColor);
				break;
			default:
				textSurface = TTF_RenderText_Solid(trebuchet, m_text.c_str(), m_fontColor);
				break;
			}
			if (!textSurface) {
				TTF_CloseFont(trebuchet);
				throw TTF_GetError();
			}

			m_textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
			if (!m_textTexture) {
				TTF_CloseFont(trebuchet);
				SDL_FreeSurface(textSurface);
				throw TTF_GetError();
			}

			TTF_CloseFont(trebuchet);
			SDL_FreeSurface(textSurface);
		}
		else {
			SDL_DestroyTexture(m_textTexture);
			m_textTexture = NULL;
		}

		int w, h;
		SDL_QueryTexture(m_textTexture, NULL, NULL, &w, &h);
		m_destination.w = w;
		m_destination.h = h;
	}
}

void Text::fontColor(SDL_Color p_fontColor) {
	if (m_fontColor.a != p_fontColor.a && m_fontColor.b != p_fontColor.b && m_fontColor.g != p_fontColor.g && m_fontColor.r != p_fontColor.r) {
		m_fontColor = p_fontColor;
		if (m_text.length()) {
			TTF_Font* trebuchet = TTF_OpenFont("ressources/trebuc.ttf", m_fontSize);
			if (!trebuchet) throw TTF_GetError();
			TTF_SetFontStyle(trebuchet, m_fontStyle);

			SDL_Surface* textSurface;
			switch (m_fontRender) {
			case eFontRender::SOLID:
				textSurface = TTF_RenderText_Solid(trebuchet, m_text.c_str(), m_fontColor);
				break;
			case eFontRender::BLENDED:
				textSurface = TTF_RenderText_Blended(trebuchet, m_text.c_str(), m_fontColor);
				break;
			default:
				textSurface = TTF_RenderText_Solid(trebuchet, m_text.c_str(), m_fontColor);
				break;
			}
			if (!textSurface) {
				TTF_CloseFont(trebuchet);
				throw TTF_GetError();
			}

			m_textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
			if (!m_textTexture) {
				TTF_CloseFont(trebuchet);
				SDL_FreeSurface(textSurface);
				throw TTF_GetError();
			}

			TTF_CloseFont(trebuchet);
			SDL_FreeSurface(textSurface);
		}
		else {
			SDL_DestroyTexture(m_textTexture);
			m_textTexture = NULL;
		}

		int w, h;
		SDL_QueryTexture(m_textTexture, NULL, NULL, &w, &h);
		m_destination.w = w;
		m_destination.h = h;
	}
}

void Text::fontSize(int p_fontSize) {
	if (m_fontSize != p_fontSize) {
		m_fontSize = p_fontSize;
		if (m_text.length()) {
			TTF_Font* trebuchet = TTF_OpenFont("ressources/trebuc.ttf", m_fontSize);
			if (!trebuchet) throw TTF_GetError();
			TTF_SetFontStyle(trebuchet, m_fontStyle);

			SDL_Surface* textSurface;
			switch (m_fontRender) {
			case eFontRender::SOLID:
				textSurface = TTF_RenderText_Solid(trebuchet, m_text.c_str(), m_fontColor);
				break;
			case eFontRender::BLENDED:
				textSurface = TTF_RenderText_Blended(trebuchet, m_text.c_str(), m_fontColor);
				break;
			default:
				textSurface = TTF_RenderText_Solid(trebuchet, m_text.c_str(), m_fontColor);
				break;
			}
			if (!textSurface) {
				TTF_CloseFont(trebuchet);
				throw TTF_GetError();
			}

			m_textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
			if (!m_textTexture) {
				TTF_CloseFont(trebuchet);
				SDL_FreeSurface(textSurface);
				throw TTF_GetError();
			}

			TTF_CloseFont(trebuchet);
			SDL_FreeSurface(textSurface);
		}
		else {
			SDL_DestroyTexture(m_textTexture);
			m_textTexture = NULL;
		}

		int w, h;
		SDL_QueryTexture(m_textTexture, NULL, NULL, &w, &h);
		m_destination.w = w;
		m_destination.h = h;
	}
}

void Text::destination(SDL_Rect p_destination) {
	m_destination = p_destination;
}

void Text::destination(int x_pos, int y_pos) {
	m_destination.x = x_pos;
	m_destination.y = y_pos;
}


//Getters
std::string Text::text(void) {
	return m_text;
}

SDL_Renderer* Text::renderer(void) {
	return m_renderer;
}

int Text::fontStyle(void) {
	return m_fontStyle;
}

eFontRender Text::fontRender(void) {
	return m_fontRender;
}

SDL_Color Text::fontColor(void) {
	return m_fontColor;
}

int Text::fontSize(void) {
	return m_fontSize;
}

SDL_Texture* Text::textTexture(void) {
	return m_textTexture;
}

SDL_Rect Text::destination(void) {
	return m_destination;
}

int Text::x_pos(void) {
	return m_destination.x;
}

int Text::y_pos(void) {
	return m_destination.y;
}

int Text::x_size(void) {
	if (m_text.length()) {
		int h, w;
		SDL_QueryTexture(m_textTexture, NULL, NULL, &w, &h);
		return w;
	}
	return 0;
}

int Text::y_size(void) {
	if (m_text.length()) {
		int h, w;
		SDL_QueryTexture(m_textTexture, NULL, NULL, &w, &h);
		return h;
	}
	return 0;
}


//Display management
void Text::render(int x_parentPos, int y_parentPos) {
	if (!m_textTexture) return;

	SDL_Rect tmp_dest;
	tmp_dest.x = x_parentPos + m_destination.x;
	tmp_dest.y = y_parentPos + m_destination.y;
	tmp_dest.w = m_destination.w;
	tmp_dest.h = m_destination.h;

	SDL_RenderCopy(m_renderer, m_textTexture, NULL, &tmp_dest);
}


//Destroyers
Text::~Text() {
	m_renderer = NULL;
	SDL_DestroyTexture(m_textTexture);
	m_textTexture = NULL;
}