#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "user.h"

class UserManager {
    public:
        UserManager() = default;

        User* authenticateLogin(std::string username, std::string password);
        bool authenticateLogout(User* user);
        bool addUser(std::string username, std::string password, Privilege privilege);
        bool removeUser(std::string username, std::string targetUsername);
        User* getUser(std::string username);

    private:
        std::vector<User*> users;

        void loadUsersFile() {}
        void appendUserFile() {}
};