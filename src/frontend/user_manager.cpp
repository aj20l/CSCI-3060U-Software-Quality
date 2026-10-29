#include "user_manager.h"

UserManager::UserManager(GameManager* gameManager) : gameManager_(gameManager) {
    loadUsersFile();
    for(Game* game : gameManager->getGames()) {
        std::cout << game->getName() << "\n";
    }
}

User* UserManager::authenticateLogin(std::string username, std::string password) {
    User* user = getUser(username);
    if (!user) {
        std::cout << "Incorrect username\n";
        return nullptr;
    }
    
    if(user->getPassword() != password) {
        std::cout << "Incorrect password\n";
        return nullptr;
    }

    if(user->getActivity()) {
        std::cout << "A session has already been started with the current user\n";
        return nullptr;
    }
    
    user->setUserActive(true);
    return user;
}

bool UserManager::authenticateLogout(User* user) {
    if (!user || !user->getActivity()) {
        std::cout << "The current user doesn't have an active session\n";
        return false;
    }
    user->setUserActive(false);
    return true;
}

bool UserManager::addUser(std::string username, std::string password, Privilege privilege) {
    User* user = getUser(username);
    if(user) {
        std::cout << "A user with the same username already exists\n";
        return false;
    }

    std::ifstream f(dataPath_);
    json data = json::parse(f);
    int uid = int(data["users"].back()["UID"]) + 1;
    f.close();
    User* newUser = new User(uid, username, password, privilege);
    users.push_back(newUser);
    appendUserFile(newUser);
    return true;
}

bool UserManager::removeUser(std::string username, std::string targetUsername) { 
    if(username == targetUsername) {
        std::cout << "Cannot delete current account being used\n";
        return false;
    }

    User* user = getUser(targetUsername);
    if(!user) {
        std::cout << "The user being deleted could not be found\n";
        return false;
    }
    
    popUserFile(user);
    for (auto it = users.begin(); it != users.end(); ++it) {
        if (*it == user) {
            delete *it;
            users.erase(it);
            break;
        }
    }
    std::cout << "User has been deleted\n";
    return true;
}

User* UserManager::getUser(std::string username) {
    for(User* user : users) {
        if(user->getUsername() == username) {
            return user;
        }
    }
    return nullptr;
}

void UserManager::loadUsersFile() {
    std::ifstream f(dataPath_);
    json data = json::parse(f);
    
    for(int i = 0; i < data["users"].size(); i++) {
        std::vector<Game*> gamesList; 
        if(!data["users"][i]["games"].is_null()) {
            for(int gid : data["users"][i]["games"].get<std::vector<int>>()) {
                Game* game = gameManager_->getGame(gid);
                if(game) {
                    gamesList.push_back(game);
                }
            } 
        }

        users.push_back(new User(data["users"][i]["UID"],
                                    data["users"][i]["username"], 
                                    data["users"][i]["password"], 
                                    data["users"][i]["privilege"],
                                    data["users"][i]["balance"],
                                    gamesList));
    }
    f.close();
}

void UserManager::popUserFile(User* user) {
    std::ifstream f(dataPath_);
    json data = json::parse(f);
    for(int i = 0; i < data["users"].size(); i++) {
        if(data["users"][i]["UID"] == user->getUID()) {
            data["users"].erase(i);
            break;
        }
    }

    std::ofstream o(dataPath_);
    o << data.dump(4);
    f.close();
    o.close();
}

void UserManager::appendUserFile(User* user) {
    std::vector<int> gid = user->getGameListID();

    json newData = {
        {"UID", user->getUID()},
        {"username", user->getUsername()},
        {"password", user->getPassword()},
        {"privilege", user->getAccountPrivilege()},
        {"balance", user->getBalance()},
        {"games", gid}
    };

    bool exists = false;
    std::ifstream f(dataPath_);
    json data = json::parse(f);
    for(int i = 0; i < data["users"].size(); i++) {
        if(data["users"][i]["UID"] == user->getUID()) {
            data["users"][i] = newData;
            exists = true;
            break;
        }
    }

    if(!exists) {
        data["users"].push_back(newData);
    }

    std::ofstream o(dataPath_);
    o << data.dump(4);
    f.close();
    o.close();
}