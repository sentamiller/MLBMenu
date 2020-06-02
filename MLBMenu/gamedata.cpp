#include "gamedata.h"
#include <cstdint>
#include <iostream>
#include <memory>
#include <curl/curl.h>
#include "json.hpp"
using json = nlohmann::json;

GameData::GameData()
{
	const std::string url("http://statsapi.mlb.com/api/v1/schedule?hydrate=game(content(editorial(recap))),decisions&date=2018-06-10&sportId=1");
	httpAndParse(url);
}

GameData::~GameData()
{
	for (Game* game : gamesList) {
		delete game;
	}
}

vector<Game*> GameData::getGames() const
{
	return gamesList;
}

namespace
{
	std::size_t callback(
		const char* in,
		std::size_t size,
		std::size_t num,
		std::string* out)
	{
		const std::size_t totalBytes(size * num);
		out->append(in, totalBytes);
		return totalBytes;
	}
}

int GameData::httpAndParse(string url)
{
	CURL* curl = curl_easy_init();

	// Set remote URL.
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

	// Don't bother trying IPv6, which would increase DNS resolution time.
	curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

	// Time out after 10 seconds.
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);

	// Follow HTTP redirects if necessary.
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

	// Response information.
	long httpCode(0);
	std::unique_ptr<std::string> httpData(new std::string());

	// Hook up data handling function.
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);

	// Hook up data container (will be passed as the last parameter to the
	// callback handling function).  Can be any pointer type, since it will
	// internally be passed as a void pointer.
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

	// Run HTTP GET command, capture the HTTP response code, and clean up.
	curl_easy_perform(curl);
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
	curl_easy_cleanup(curl);

	if (httpCode == 200)
	{
		// Parse the JSON data in the GET response
		auto jsonResponse = json::parse(*httpData.get());
		json datesArray = jsonResponse["dates"][0];
		json gamesArray = datesArray["games"];

		for (json game : gamesArray) {
			Game* item = new Game();

			item->awayTeam = game["teams"]["away"]["team"]["name"].get<std::string>();
			item->homeTeam = game["teams"]["home"]["team"]["name"].get<std::string>();
			item->awayScore = game["teams"]["away"]["score"];
			item->homeScore = game["teams"]["home"]["score"];

			json contentHome = game["content"]["editorial"]["recap"]["home"];
			item->headline = contentHome["headline"].get<std::string>();
			item->imageUrl = contentHome["photo"]["cuts"]["248x138"]["src"].get<std::string>();

			gamesList.push_back(item);
		}
	}
	else
	{
		std::cout << "Could not GET from " << url << " - exiting" << std::endl;
		return 1;
	}
	return 0;
}

void GameData::printGamesList() const {
	for (Game* item : gamesList) {
		std::cout << "item->awayTeam  = " << item->awayTeam << "\n";
		std::cout << "item->homeTeam  = " << item->homeTeam << "\n";
		std::cout << "item->awayScore = " << item->awayScore << "\n";
		std::cout << "item->homeScore = " << item->homeScore << "\n";
		std::cout << "item->headline  = " << item->headline << "\n";
		std::cout << "item->imageUrl  = " << item->imageUrl << "\n";
	}
}
