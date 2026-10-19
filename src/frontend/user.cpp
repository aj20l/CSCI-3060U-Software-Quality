#include "user.h"

User::User(std::string username, std::string password, Privilege privilege) 
            : username_(username), password_(password), privilege_(privilege) {}


void User::appendGame(Game* game) {
    gamesList_.push_back(game);
}

void User::removeGame(Game* game) {
    auto it = std::find(gamesList_.begin(), gamesList_.end(), game);
    if (it != gamesList_.end()) {
        gamesList_.erase(it);
    }
}

bool User::hasGame(std::string gameName, std::string sellersName) {
    for(Game* game : gamesList_){
        if(game->getName() == gameName && game->getSeller() == sellersName) {
            return true;
        }
    }   
    return false;
}