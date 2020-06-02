#include "text.h"
#include <iostream>

Text::Text(SDL_Renderer* renderer, 
	const std::string& fontPath, int fontSize, 
	const std::string& messageText, const SDL_Color& color)
{
	texture = loadFont(renderer, fontPath, fontSize, messageText, color);
	SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
}

void Text::display(SDL_Renderer* renderer, int x, int y) const
{
	rect.x = x;
	rect.y = y;
	SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

SDL_Texture* Text::loadFont(SDL_Renderer* renderer, 
	const std::string& fontPath, int fontSize, 
	const std::string& messageText, const SDL_Color& color)
{
	TTF_Font* font = TTF_OpenFont(fontPath.c_str(), fontSize);
	if (!font) {
	std::cerr << "Failed to load font. \n";
	}

	auto surface = TTF_RenderText_Blended(font, messageText.c_str(), color);
	if (!surface) {
	std::cerr << "Failed to create text surface. \n";
	}

	auto texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) {
		std::cerr << "Failed to create text texture. \n";
	}

	TTF_CloseFont(font);
	SDL_FreeSurface(surface);
	return texture;
}

int Text::getWidth() const
{
	return rect.w;
}

int Text::getHeight() const
{
	return rect.h;
}
