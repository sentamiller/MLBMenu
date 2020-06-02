#pragma once

#include "window.h"
#include <vector>
#include "rect.h"
#include "text.h"
using namespace std;

struct GameDetails {
	Text* awayLine;
	Text* homeLine;
	Text* awayScore;
	Text* homeScore;
};

class MenuManager
{
public:
	MenuManager(int windowWidth, int windowHeight);
	~MenuManager();

	void pollEvents(SDL_Event& event);
	void display();

private:
	vector<Rect*> rectList;
	vector<Text*> headlines;
	vector<GameDetails*> details;

	int urlToRect(string url, SDL_Rect pos);

};

