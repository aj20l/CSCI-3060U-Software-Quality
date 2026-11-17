// This header file defines the SessionManager class, which manages user sessions in the system. It keeps track of the currently logged-in user and provides methods to start and end sessions, check login status, and determine if the current user is an admin.
#pragma once

#include <map>
#include <string>
#include "User.h"
using namespace std;

class SessionManager {
    public:
        // Initializes the SessionManager with no user logged in
        SessionManager() : currentUser("", "", 0.0), loggedIn(false) {}

        // Called at login: remember the user and mark them as logged in. Also clears any added credit for the new session.
        void startSession(const User& user) {
            currentUser = user;
            loggedIn = true;
            addedCredit.clear();
        }

        // Called at logout: forget the user and mark them as logged out. Also clears any added credit for the session.
        void endSession() {
            currentUser = User("", "", 0.0);
            loggedIn = false;
            addedCredit.clear();
        }

        bool isLoggedIn() const { return loggedIn; }

        // Returns true if the current user is an admin, false otherwise. If no user is logged in, returns false
        bool isAdmin() const { return loggedIn && currentUser.isAdmin(); }

        const User& getCurrentUser() const { return currentUser; }

        // Records the amount of credit added to the current user's account during this session
        double getAddedCredit(const string& username) const {
            auto entry = addedCredit.find(username);
            return (entry == addedCredit.end()) ? 0.0 : entry->second;
        }

        // Records the amount of credit added to the current user's account during this session
        void recordAdded(const string& username, double amount) { addedCredit[username] += amount; }

    private:
        User currentUser;
        bool loggedIn;
        map<string, double> addedCredit;
};