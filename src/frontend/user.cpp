#include <iostream>
#include <string>
#include <vector>
#include "game.cpp"

enum Privilege{
    ADMIN,
    FULLSTANDARD,
    BUYSTANDARD,
    SELLSTANDARD
};

class User {
    public:
        User() = default;
		User(std::string username, std::string password, Privilege privilege) 
            : username_(username), password_(password), privilege_(privilege) {}
		
        std::string getUsername() { return username_; }
        std::string getPassword() { return password_; }
        float getBalance() { return balance_; }
        void setBalance(float balance) { balance_ = balance; } // could change to return bool
        Privilege getAccountPrivilege() { return privilege_; }
        void setUserActive() { isActive_ = !isActive_; }
        bool getActivity() { return isActive_; }
        
        void appendGame(Game* game) { gamesList_.push_back(game); }
        void removeGame(Game* game) {};
        bool hasGame(std::string gameName, std::string sellersName) { 
            for(Game* game : gamesList_){
                if(game->getName() == gameName && game->getSeller() == sellersName) {
                    return true;
                }
            }   
            return false;
        }
        std::vector<Game*> getGamesList() { return gamesList_; }

    private:
        std::string UID_;
        std::string username_;
        std::string password_;
        Privilege privilege_;
        float balance_;
        bool isActive_ = false;
        std::vector<Game*> gamesList_;

};