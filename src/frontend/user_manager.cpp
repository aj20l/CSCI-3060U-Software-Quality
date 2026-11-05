#include "user_manager.h"

UserManager::UserManager(GameManager* gameManager) : gameManager_(gameManager) {
    loadUsersFile();
}

// Authenticates login, mapping data to an existing user in the system
// if they exists
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

// Authenticates a user can logout only if that user is active
bool UserManager::authenticateLogout(User* user) {
    if (!user || !user->getActivity()) {
        std::cout << "The current user doesn't have an active session\n";
        return false;
    }
    user->setUserActive(false);
    return true;
}

// Adds a new user to the system and user data file
bool UserManager::addUser(std::string username, std::string password, Privilege privilege) {
    User* user = getUser(username);
    if(user) {
        std::cout << "A user with the same username already exists\n";
        return false;
    }

    // Obtains the newest user in the system and increments their id
    // for to be used by the current user being created
    std::ifstream f(dataPath_);
    json data = json::parse(f);
    int uid = int(data["users"].back()["UID"]) + 1;
    f.close();
    User* newUser = new User(uid, username, password, privilege);
    
    // Add the new user into the system before appending the user
    // data file with the new user
    users.push_back(newUser);
    appendUserFile(newUser);
    return true;
}

// Removes user from system and user data file
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

    // Removes the targets user from the system before removing the
    // targeted from the user data file
    for (auto it = users.begin(); it != users.end(); ++it) {
        if (*it == user) {
            delete *it;
            users.erase(it);
            break;
        }
    }
    popUserFile(user);
    std::cout << "User has been deleted\n";
    return true;
}

// Returns a user if they exists in the system
User* UserManager::getUser(std::string username) {
    for(User* user : users) {
        if(user->getUsername() == username) {
            return user;
        }
    }
    return nullptr;
}

// Loads users from user data file
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

// Removes a given user from the user data file
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

// Appends user data file given a user, adding new user or editing
// their information if they exist
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