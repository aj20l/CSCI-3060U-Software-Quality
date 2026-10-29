#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include "game.h"
#include "utils.h"
#include <iostream>

class User {
    public:
		User(int uid, std::string username, std::string password, Privilege privilege);
        User(int uid, std::string username, std::string password, Privilege privilege, float balance, std::vector<Game*> gamesList);

        int getUID() const { return UID_; }
        std::string getUsername() const { return username_; }
        std::string getPassword() const { return password_; }
        float getBalance() const { return balance_; }
        void setBalance(float balance) { balance_ = balance; }
        Privilege getAccountPrivilege() const { return privilege_; }
        bool getActivity() const { return isActive_; }
        void setUserActive(bool active) { isActive_ = active; }
        
        std::vector<Game*> getGamesList() const { return gamesList_; }
        std::vector<int> getGameListID() {
            std::vector<int> ids;
            for(Game* game: gamesList_) {
                ids.push_back(game->getGID());
            }
            return ids;
        }
        bool hasGame(std::string gameName, std::string sellersName);
        void appendGame(Game* game);
        void removeGame(Game* game);

    private:
        int UID_;
        std::string username_;
        std::string password_;
        Privilege privilege_;
        float balance_ = 0.0F;
        bool isActive_ = false;
        std::vector<Game*> gamesList_;

};