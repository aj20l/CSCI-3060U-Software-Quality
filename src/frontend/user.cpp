#include "user.h"

User::User(int uid, std::string username, std::string password, Privilege privilege) 
    : UID_(uid), username_(username), password_(password), privilege_(privilege) {}

User::User(int uid, std::string username, std::string password, Privilege privilege, float balance, std::vector<Game*> gamesList)
    :  UID_(uid), username_(username), password_(password), privilege_(privilege), balance_(balance), gamesList_(gamesList) {}

// Appends a game from the users game list
void User::appendGame(Game* game) {
    gamesList_.push_back(game);
}

// Removes a game from the users games list
void User::removeGame(Game* game) {
    auto it = std::find(gamesList_.begin(), gamesList_.end(), game);
    if (it != gamesList_.end()) {
        gamesList_.erase(it);
    }
}

// Checks if the user has a given game in the games list
bool User::hasGame(std::string gameName, std::string sellersName) {
    for(Game* game : gamesList_){
        if(game->getName() == gameName && game->getSeller() == sellersName) {
            return true;
        }
    }   
    return false;
}