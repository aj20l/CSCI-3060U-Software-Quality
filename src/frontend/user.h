#include <string>
#include <vector>
#include <algorithm>
#include "game.h"

enum Privilege {
    ADMIN,
    FULLSTANDARD,
    BUYSTANDARD,
    SELLSTANDARD
};

class User {
    public:
        User() = default;
		User(std::string username, std::string password, Privilege privilege);
		
        std::string getUsername() const { return username_; }
        std::string getPassword() const { return password_; }
        float getBalance() const { return balance_; }
        void setBalance(float balance) { balance_ = balance; }
        Privilege getAccountPrivilege() const { return privilege_; }
        bool getActivity() const { return isActive_; }
        void setUserActive(bool active) { isActive_ = active; }
        
        std::vector<Game*> getGamesList() const { return gamesList_; }
        bool hasGame(std::string gameName, std::string sellersName);
        void appendGame(Game* game);
        void removeGame(Game* game);

    private:
        std::string UID_;
        std::string username_;
        std::string password_;
        Privilege privilege_;
        float balance_ = 0.0F;
        bool isActive_ = false;
        std::vector<Game*> gamesList_;

};