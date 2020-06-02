#include "menumanager.h"
#include "curl/curl.h"
#include "gamedata.h"
#include <algorithm>
#include <iostream>

// FOCUS_TARGET is the position of the game data that will be expanded
const int FOCUS_TARGET = 3;

int rectWidth;
int rectHeight;
int space;
int focusPosition;

MenuManager::MenuManager(int windowWidth, int windowHeight)
{
	// Load game data
	GameData gameInfo = GameData();

	// Dimensions and positions for rect objects
	rectWidth = windowWidth / 10;
	rectHeight = windowHeight / 10;
	space = rectWidth / 3;
	int leftStart = windowWidth / 20;
	int topStart = windowHeight / 3;

	// Font sizes
	int headlineFontSize = windowHeight / 27;
	int detailsFontSize = windowHeight / 36;

	int i = 0;
	for (Game* game : gameInfo.getGames()) {
		int currentX = (rectWidth + space) * i + leftStart;

		// Update Rect list with image from url
		urlToRect(game->imageUrl, { currentX, topStart, rectWidth, rectHeight });

		// Set text for headline
		string headline = game->headline;
		// Limit size of headline
		if (headline.length() > 50) {
			headline = headline.substr(0, 50) + "...";
		}
		headlines.push_back(new Text(Window::renderer, "fonts/arial.ttf", headlineFontSize, headline, { 255, 255, 0, 255 }));

		// Set Text for other game info 
		GameDetails* item = new GameDetails();
		item->awayLine = new Text(Window::renderer, "fonts/arial.ttf", detailsFontSize, game->awayTeam, { 225, 127, 0, 255 });
		item->homeLine = new Text(Window::renderer, "fonts/arial.ttf", detailsFontSize, game->homeTeam, { 225, 127, 50, 255 });
		item->awayScore = new Text(Window::renderer, "fonts/arial.ttf", detailsFontSize, to_string(game->awayScore), { 225, 127, 0, 255 });
		item->homeScore = new Text(Window::renderer, "fonts/arial.ttf", detailsFontSize, to_string(game->homeScore), { 225, 127, 50, 255 });
		details.push_back(item);

		i++;
		if (i == FOCUS_TARGET) {
			focusPosition = currentX;
		}
	}
	if (focusPosition == 0) {
		focusPosition = leftStart;
	}
}

MenuManager::~MenuManager()
{
	for (Rect* rect : rectList) {
		delete rect;
	}
	for (Text* text : headlines) {
		delete text;
	}
	for (GameDetails* detail : details) {
		delete detail->awayLine;
		delete detail->awayScore;
		delete detail->homeLine;
		delete detail->homeScore;
		delete detail;
	}
}

void MenuManager::pollEvents(SDL_Event& event)
{
	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
		case SDLK_LEFT:
			// Do not move to left if currently at the beginning
			if (rectList.front()->getX() != focusPosition) {
				for (Rect* rect : rectList) {
					rect->shiftX(rectWidth + space);
				}
			}
			break;
		case SDLK_RIGHT:
			// Do not move to right if currently at the end
			if (rectList.back()->getX() != focusPosition) {
				for (Rect* rect : rectList) {
					rect->shiftX(-(rectWidth + space));
				}
			}
			break;
		}
	}
}

void MenuManager::display() 
{
	for (int i = 0; i < rectList.size(); i++) {
		if (rectList[i]->getX() == focusPosition) {
			rectList[i]->draw(true);

			// Above thumbnail text
			headlines[i]->display(Window::renderer, 
				rectList[i]->getX() + rectList[i]->getW() / 2 - headlines[i]->getWidth() / 2, 
				rectList[i]->getY() - rectList[i]->getH());
			
			// Below thumbnail text
			int textLeft = rectList[i]->getX() - rectList[i]->getW() / 3;
			int textTop = rectList[i]->getY() + rectList[i]->getH();
			int scoreOffset = max(details[i]->awayLine->getWidth(), details[i]->homeLine->getWidth()) + rectList[i]->getW() / 5;

			details[i]->awayLine->display(Window::renderer, textLeft, textTop + details[i]->homeLine->getHeight() * 1.1);
			details[i]->awayScore->display(Window::renderer, textLeft + scoreOffset, textTop + details[i]->homeLine->getHeight() * 1.1);
			details[i]->homeLine->display(Window::renderer, textLeft, textTop + details[i]->homeLine->getHeight() * 2.2);
			details[i]->homeScore->display(Window::renderer, textLeft + scoreOffset, textTop + details[i]->homeLine->getHeight() * 2.2);
		}
		else {
			rectList[i]->draw();
		}
	}
}

struct MemoryStruct {
	char* memory;
	size_t size;
};

static size_t
WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct* mem = (struct MemoryStruct*)userp;

	char* ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
	if (ptr == NULL) {
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

int MenuManager::urlToRect(string url, SDL_Rect pos)
{
	CURL* curl_handle;
	CURLcode res;

	struct MemoryStruct chunk;

	chunk.memory = (char*)malloc(1);
	chunk.size = 0;

	curl_global_init(CURL_GLOBAL_ALL);

	curl_handle = curl_easy_init();

	const char* cstr = url.c_str();
	curl_easy_setopt(curl_handle, CURLOPT_URL, cstr);

	// Send all data to this function 
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&chunk);
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);

	// Perform Get
	res = curl_easy_perform(curl_handle);

	if (res != CURLE_OK) {
		// Error occurred.  Use default image.
		rectList.push_back(new Rect(pos.x, pos.y, pos.w, pos.h, "images/defaultBaseball.jpg"));
	}
	else {
		rectList.push_back(new Rect(pos.x, pos.y, pos.w, pos.h, chunk.memory, chunk.size));
	}

	curl_easy_cleanup(curl_handle);
	free(chunk.memory);
	curl_global_cleanup();

	return 0;
}
