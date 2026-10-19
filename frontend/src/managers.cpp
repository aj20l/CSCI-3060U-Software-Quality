#include <iostream>
#include <string>
#include <optional>
#include "game_manager.cpp"

enum Privilege{
    ADMIN,
    FULLSTANDARD,
    BUYSTANDARD,
    SELLSTANDARD
};


class User {
    public:
        User() = default;
		User(std::string username,
        std::string password,
        Privilege privilege){
        	this->username=username;
        	this->password=password;
        	this->privilege=privilege;
        	
		}
		
        std::string getUsername() { return username; }
        bool checkUsername(std::string username) { return true; }
        bool checkPassword(std::string password) { return true; }
        float getBalance() { return balance; }
        void setBalance(float amount) { balance += amount; } // could change to return bool
        Privilege getAccountPrivilege() { return privilege; }
        void setUserActive() { isUserActive = !isUserActive; }
        bool isActive() { 
            return isUserActive; 
        }
        
        bool hasGame(Game game) {
		
		return false;}

    private:
        std::string UID;
        std::string username;
        std::string password;
        Privilege privilege;
        float balance;
        bool isUserActive = false;
        Game games[1];

};

class UserManager {
    public:
        UserManager() = default;

        bool createUser(std::string username, std::string password, Privilege privilege) {
        	User newUser= User(username,password,privilege);
        	newUser.setUserActive();
			if(newUser.isActive()){
				std::cout<<newUser.getUsername()+" has been created";
				
				//adds user to db
				return true;
			}
			return false;
        }
        bool deleteUser(std::string username) { return true; }
        std::optional<User> getUser(std::string username) {
            // add some kind of search function to find the user
            return std::nullopt;
        }

    private:
        User users[10];

        void loadUsersFile() {}
};