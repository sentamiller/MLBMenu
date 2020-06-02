#pragma once

#include <SDL_ttf.h>
#include <SDL.h>
#include <string>

class Text
{
public:
	Text(SDL_Renderer* renderer, 
		const std::string& fontPath, int fontSize, 
		const std::string& messageText, const SDL_Color& color);

	void display(SDL_Renderer* renderer, int x, int y) const;

	static SDL_Texture* loadFont(SDL_Renderer* renderer, 
		const std::string& fontPath, int fontSize,
		const std::string& messageText, const SDL_Color& color);

	int getWidth() const;
	int getHeight() const;

private:
	SDL_Texture* texture = nullptr;
	mutable SDL_Rect rect;
};

