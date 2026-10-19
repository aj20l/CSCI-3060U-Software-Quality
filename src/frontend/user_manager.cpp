#include <iostream>
#include <string>
#include <vector>
#include "user.cpp"

class UserManager {
    public:
        UserManager() = default;

        User* authenticateLogin(std::string username, std::string password) {
            User* user = getUser(username);
            if(user) {
                std::cout << "Incorrect username\n";
                return NULL;
            }
            
            if(user->getPassword() != password) {
                std::cout << "Incorrect password\n";
                return NULL;
            }

            if(user->getActivity()) {
                std::cout << "A session has already been started with the current user\n";
                return NULL;
            }
            
            user->setUserActive();
            return user;
        }
        bool authenticateLogout(User* user) {
            if(!user->getActivity()) {
                std::cout << "The current user doesn't have an active session\n";
                return false;
            }
            user->setUserActive();
            return true;
        }
        bool addUser(std::string username, std::string password, Privilege privilege) {
            User* user = getUser(username);
            if(user) {
                std::cout << "A user with the same username already exists\n";
                return false;
            }

        	User* newUser = new User(username, password, privilege);
            users.push_back(newUser);
			return true;
        }
        bool removeUser(std::string username, std::string targetUsername) { 
            if(username == targetUsername) {
                std::cout << "Cannot delete current account being used\n";
                return false;
            }

            User* user = getUser(targetUsername);
            if(!user) {
                std::cout << "The user being deleted could not be found\n";
                return false;
            }
            
            // code for deleting user
            std::cout << "User has been deleted\n";
            return true;
        }
        User* getUser(std::string username) {
            for(User* user : users) {
                if(user->getUsername() == username) {
                    return user;
                }
            }
            return NULL;
        }
        bool changeUserBalance(User* user, float amount) {

        }

    private:
        std::vector<User*> users;

        void loadUsersFile() {};
        void appendUserFile() {};
};