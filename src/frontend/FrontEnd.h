// This header file defines the FrontEnd class, which manages user interactions, including login, logout, and account management. It uses FileManager for file operations, SessionManager for session handling, and UserStore for managing user data.
#pragma once

#include <iostream>
#include <string>
#include "FileManager.h"
#include "SessionManager.h"
#include "UserStore.h"
#include "GameStore.h"
using namespace std;

class FrontEnd {
    public:
        // Constructor initializes the FrontEnd with input/output streams and a FileManager for file operations
        FrontEnd(istream& input, ostream& output, const FileManager& files);

        // Runs the main loop of the front-end, handling user commands until the program is terminated
        void run();

    private:
        // Decides what to do with one command typed by the user
        void handleCommand(const string& command);

        // One function per transaction
        void handleLogin();
        void handleLogout();
        void handleListAccounts();   // privileged, admin only
        void handleCreate();         // privileged, admin only
        void handleDelete();         // privileged, admin only
        void handleSell();
        void handleBuy();
        void handleRefund();         // privileged, admin only
        void handleAddCredit();
        void handleListGames();
        void handleListCollections();
        void handleSearch();

        bool isPermitted(const string& command) const;

        // Helper functions for user interaction
        bool promptForInput(const string& message, string& response);
        void displayMessage(const string& message);
        void displayError(const string& message);

        istream& input;        
        ostream& output;       
        FileManager files;     
        SessionManager session;
        UserStore users;
        GameStore games;
};