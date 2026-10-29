#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "json.hpp"
#include "user.h"
#include "utils.h"
#include "game_manager.h"

using nlohmann::json;

class UserManager {
    public:
        UserManager(GameManager* gameManager);

        User* authenticateLogin(std::string username, std::string password);
        bool authenticateLogout(User* user);
        bool addUser(std::string username, std::string password, Privilege privilege);
        bool removeUser(std::string username, std::string targetUsername);
        User* getUser(std::string username);
        std::vector<User*> getUserList() { return users; }
        void appendUserFile(User* user);
        void popUserFile(User* user);

    private:
        GameManager* gameManager_ = nullptr;
        std::vector<User*> users;
        std::string dataPath_ = "./src/data/user_data.json";

        void loadUsersFile();
};