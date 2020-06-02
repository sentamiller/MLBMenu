#pragma once
#include <string>
#include <vector>

using namespace std;

struct Game {
	string headline;
	string awayTeam;
	string homeTeam;
	int awayScore;
	int homeScore;
	string imageUrl;
};

class GameData
{
public:
	GameData();
	~GameData();

	vector<Game*> getGames() const;
	int httpAndParse(string url);
	void printGamesList() const;

private:
	vector<Game*> gamesList;

};
