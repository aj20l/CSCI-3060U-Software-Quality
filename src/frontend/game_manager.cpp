#include <iostream>
#include "game_manager.h"

GameManager::GameManager() {
    loadGamesFile();
}

// Add a new game to the system
bool GameManager::addGame(std::string gameName, std::string gameSeller, float gamePrice) {
    Game* game = getGame(gameName, gameSeller);

    // Checks if the game exists
    if(game) {
        return false;
    }

    // Incrementes most revent game id to be used by the current game
    // being added
    std::ifstream f(dataPath_);
    json data = json::parse(f);
    int uid = int(data["games"].back()["GID"]) + 1;
    f.close();

    // Creates the new game, pushing it into the system before the
    // game data file
    Game* newGame = new Game(uid, gameName, gameSeller, gamePrice);
    games.push_back(newGame);
    appendGameFile(newGame);
    return true;
}

// Removes game from system and game data file
bool GameManager::removeGame(std::string gameName, std::string gameSeller) {
    for (auto it = games.begin(); it != games.end(); ++it) {
        if ((*it)->getName() == gameName && (*it)->getSeller() == gameSeller) {
            delete *it;
            games.erase(it);
            return true;
        }
    }

    std::cout << "Game being deleted doesn't exist\n";
    return false;
}

// Gets a game from the system for a games given its name and seller
Game* GameManager::getGame(std::string gameName, std::string gameSeller) {
    for(Game* game : games) {
        if(game->getName() == gameName && game->getSeller() == gameSeller) {
            return game;
        }
    }
    return NULL;
}

// Gets a game from the system for a games given id
Game* GameManager::getGame(int gid) {
    for(Game* game : games) {
        if(game->getGID() == gid) {
            return game;
        }
    }
    return nullptr;
}

// Loads games from game data file
void GameManager::loadGamesFile() {
    std::ifstream f(dataPath_);
    json data = json::parse(f);
    
    for(int i = 0; i < data["games"].size(); i++) {
        games.push_back(new Game(data["games"][i]["GID"],
                                    data["games"][i]["gameName"], 
                                    data["games"][i]["gameSeller"], 
                                    data["games"][i]["price"]));
    }
    f.close();
}

// Appends games data file given a game, adding a new game or
// editing the data of an existing game
void GameManager::appendGameFile(Game* game) {
    json newData = {
        {"GID", game->getGID()},
        {"gameName", game->getName()},
        {"gameSeller", game->getSeller()},
        {"price", game->getGamePrice()},
    };
    std::ifstream f(dataPath_);
    json data = json::parse(f);
    data["games"].push_back(newData);
    std::ofstream o(dataPath_);
    o << data.dump(4);
    f.close();
    o.close();
}