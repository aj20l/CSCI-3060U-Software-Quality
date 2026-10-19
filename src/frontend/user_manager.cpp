#include "user_manager.h"

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

    User* newUser = new User(username, password, privilege);
    users.push_back(newUser);
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