#include "rect.h"
#include <SDL_image.h>
#include <iostream>

Rect::Rect(int x, int y, int w, int h, int r, int g, int b, int a) :
	x(x), y(y), w(w), h(h), r(r), g(g), b(b), a(a)
{}

Rect::Rect(int x, int y, int w, int h, const std::string& imagePath) :
	x(x), y(y), w(w), h(h), r(100), g(100), b(100), a(255)
{
	SDL_Surface* loadedSurface = IMG_Load(imagePath.c_str());
	if (loadedSurface == NULL) {
		std::string defaultRect = "images/defaultBaseball.jpg";
		loadedSurface = IMG_Load(defaultRect.c_str());
	}
	if (loadedSurface == NULL) {
			std::cerr << "Unable to load the image: " << imagePath.c_str() <<
			" - SDL_image Error: " << IMG_GetError() << "\n";
	}
	else {
		if (Window::renderer == nullptr) {
		std::cout << "null renderer for rect\n";
		}
		texture = SDL_CreateTextureFromSurface(Window::renderer, loadedSurface);
		if (texture == NULL) {
			std::cerr << "Unable to create texture from " << imagePath.c_str()
				<< " - SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(loadedSurface);
	}	
}

Rect::Rect(int x, int y, int w, int h, char* imgBuffer, size_t bufferSize) :
x(x), y(y), w(w), h(h), r(100), g(100), b(100), a(255)
{
	SDL_RWops* rw = SDL_RWFromMem(imgBuffer, bufferSize);
	SDL_Surface* loadedSurface = IMG_Load_RW(rw, 1);
	if (loadedSurface == NULL) {
		std::string defaultRect = "images/defaultBaseball.jpg";
		loadedSurface = IMG_Load(defaultRect.c_str());
	}
	if (loadedSurface == NULL) {
		std::cerr << "Unable to load the image - SDL_image Error: " << IMG_GetError() << "\n";
	}
	else {
		if (Window::renderer == nullptr) {
			std::cout << "null renderer for rect\n";
		}
		texture = SDL_CreateTextureFromSurface(Window::renderer, loadedSurface);
		if (texture == NULL) {
			std::cerr << "Unable to create texture from - SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(loadedSurface);
	}
}

Rect::~Rect() 
{
	SDL_DestroyTexture(texture);
}

void Rect::draw(bool focus) const
{
	SDL_Rect rect = { x, y, w, h };
	if (focus) {
		rect = { x - (w / 4), y - (h / 4), w * 3/2, h * 3/2 };
	}

	if (texture) {
		if (!Window::renderer) { printf("renderer is null\n"); }
		SDL_RenderCopy(Window::renderer, texture, nullptr, &rect);
	} 
	else {
		SDL_SetRenderDrawColor(Window::renderer, r, g, b, a);
		SDL_RenderFillRect(Window::renderer, &rect);
	}
}

void Rect::shiftX(int shift)
{
	x += shift;
}
