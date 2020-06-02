#pragma once

#include <SDL.h>
#include <string>

class Window
{
public:
	inline Window() {}
	Window(const std::string& title, int width, int height);
	~Window();

	void pollEvents(SDL_Event& event);
	void clear() const;
	void loadingScreen() const;

	SDL_Texture* LoadTexture(std::string file) const;
	inline bool isClosed() const { return closed; }

private:
	bool init();

	std::string title;
	int width = 1920;
	int height = 1080;

	bool closed = false;

	SDL_Window* window = nullptr;
	SDL_Texture* texture = nullptr;
	SDL_Texture* loadingTexture = nullptr;

public:
	static SDL_Renderer* renderer;

};

