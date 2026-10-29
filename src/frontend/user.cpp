#include "user.h"

User::User(int uid, std::string username, std::string password, Privilege privilege) 
    : UID_(uid), username_(username), password_(password), privilege_(privilege) {}

User::User(int uid, std::string username, std::string password, Privilege privilege, float balance, std::vector<Game*> gamesList)
    :  UID_(uid), username_(username), password_(password), privilege_(privilege), balance_(balance), gamesList_(gamesList) {}



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