#include <iostream>
#include "game_manager.h"

bool GameManager::addGame(std::string gameName, std::string gameSeller, float gamePrice) {
    Game* game = getGame(gameName, gameSeller);
    if(game) {
        std::cout << "A game by the seller with the same name already exits\n";
        return false;
    }

    Game* newGame = new Game(gameName, gameSeller, gamePrice);
    games.push_back(newGame);
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