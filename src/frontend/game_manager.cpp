#include <iostream>
#include "game_manager.h"

GameManager::GameManager() {
    loadGamesFile();
}

bool GameManager::addGame(std::string gameName, std::string gameSeller, float gamePrice) {
    Game* game = getGame(gameName, gameSeller);
    if(game) {
        std::cout << "A game by the seller with the same name already exits\n";
        return false;
    }

    std::ifstream f(dataPath_);
    json data = json::parse(f);
    int uid = int(data["games"].back()["GID"]) + 1;
    f.close();
    Game* newGame = new Game(uid, gameName, gameSeller, gamePrice);
    games.push_back(newGame);
    appendGameFile(newGame);
    return true;
}

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

Game* GameManager::getGame(std::string gameName, std::string gameSeller) {
    for(Game* game : games) {
        if(game->getName() == gameName && game->getSeller() == gameSeller) {
            return game;
        }
    }
    std::cout << "Could not find game\n";
    return NULL;
}

Game* GameManager::getGame(int gid) {
    for(Game* game : games) {
        if(game->getGID() == gid) {
            return game;
        }
    }
    return nullptr;
}

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