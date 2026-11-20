// This header file defines the UserStore class, which manages a collection of User objects. It provides methods to load users from a file and find users by their username.
#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include "FileManager.h"
#include "User.h"
using namespace std;

class UserStore {
    public:
        // Asks filemanager to read the accounts file and keep the result
        bool loadUsers(const FileManager& files) { return files.readAccounts(users); }

        // Finds a user by username and returns a pointer to the User object, or nullptr if not found
        const User* findUser(const string& username) const {
            for (const User& user : users) {
                if (user.getUsername() == username) return &user;
            }
            return nullptr;
        }

        // Finds a user by username and returns a pointer to the User object, or nullptr if not found
        User* findUser(const string& username) {
            for (User& user : users) {
                if (user.getUsername() == username) return &user;
            }
            return nullptr;
        }

        // Adds a new user to the store
        void addUser(const User& user) { users.push_back(user); }

        // Removes a user from the store by username
        void removeUser(const string& username) {
            users.erase(
                remove_if(users.begin(), users.end(), [&](const User& user) { return user.getUsername() == username; }),
                users.end()
            );
        }
        
        // Returns a reference to the vector of all users
        const vector<User>& getUsers() const { return users; }

    private:
        vector<User> users;
};