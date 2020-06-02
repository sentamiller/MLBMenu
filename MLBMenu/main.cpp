#include "window.h"
#include "menumanager.h"

// Modify WINDOW_WIDTH to change the menu display size
const int WINDOW_WIDTH = 960;

// WINDOW_HEIGHT is set to maintain a 16:9 aspect ratio
const int WINDOW_HEIGHT = WINDOW_WIDTH * 9 / 16;

void pollEvents(Window& window, MenuManager& menu)
{
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		menu.pollEvents(event);
		window.pollEvents(event);
	}
}

int main(int argc, char** argv)
{
	Window window("MLB Menu", WINDOW_WIDTH, WINDOW_HEIGHT);
	window.loadingScreen();

	MenuManager menu(WINDOW_WIDTH, WINDOW_HEIGHT);

	while (!window.isClosed()) {
		pollEvents(window, menu);

		menu.display();
		window.clear();

	}

	return 0;
}