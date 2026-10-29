#pragma once
#include <vector>
#include <fstream>
#include "json.hpp"
#include "game.h"

using nlohmann::json;

class GameManager {
public:
    GameManager();

    bool addGame(std::string gameName, std::string gameSeller, float gamePrice);
    bool removeGame(std::string gameName, std::string gameSeller);
    Game* getGame(std::string gameName, std::string gameSeller);
    Game* getGame(int gid);

    std::vector<Game*> getGames() { return games; };

private:
    std::vector<Game*> games;
    std::string dataPath_ = "./src/data/game_data.json";

    void loadGamesFile();
    void appendGameFile(Game* game);
};