#include <vector>
#include "game.h"

class GameManager {
public:
    GameManager() = default;

    bool addGame(std::string gameName, std::string gameSeller, float gamePrice);
    bool removeGame(std::string gameName, std::string gameSeller);
    Game* getGame(std::string gameName, std::string gameSeller);
    std::vector<Game*> getGames() { return games; };

private:
    std::vector<Game*> games;

    void loadGamesFile() {}
    void appendGameFile() {}
};