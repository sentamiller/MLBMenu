#include "window.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

SDL_Renderer* Window::renderer = nullptr;

Window::Window(const std::string& title, int width, int height) :
	title(title), width(width), height(height)
{
	closed = !init();
}

Window::~Window()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool Window::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "Failed to initialize SDL. \n";
		return false;
	}

	int imgFlags = IMG_INIT_JPG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		std::cerr << "Failed to initialize SDL_image. \n";
		return false;
	}

	if (TTF_Init() == -1) {
		std::cerr << "Failed to initialize SDL_ttf. \n";
		return false;
	}

	window = SDL_CreateWindow(title.c_str(),
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height, SDL_WINDOW_SHOWN);

	if (window == nullptr) {
		std::cerr << "Window could not be created! \n";;
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		std::cerr << "Not able to create renderer. \n";
		return false;
	}

	texture = LoadTexture("images/mlbBackground.jpg");
	if (texture == nullptr) {
		std::cerr << "Failed to load background image. \n";
		return false;
	}

	loadingTexture = LoadTexture("images/loadingScreen.jpg");
	if (texture == nullptr) {
		std::cerr << "Failed to load background image. \n";
		return false;
	}

	return true;
}

void Window::pollEvents(SDL_Event& event)
{
	switch (event.type) {
	case SDL_QUIT:
		closed = true;
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
		case SDLK_ESCAPE:
			closed = true;
			break;
		}
	}
}

void Window::clear() const
{
	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
}

void Window::loadingScreen() const
{
	SDL_RenderCopy(renderer, loadingTexture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

SDL_Texture* Window::LoadTexture(std::string file) const
{
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(file.c_str());
	if (loadedSurface == NULL) {
		std::cerr << "Unable to load the image: " << file.c_str() << 
			" - SDL_image Error: " << IMG_GetError() << "\n";
	}
	else {
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

		if (newTexture == NULL) {
			std::cerr << "Unable to create texture from " << file.c_str() 
				<< " - SDL Error: " << SDL_GetError() << "\n";
	    }
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}