// This header file defines the FileManager class, which is responsible for managing file operations related to user accounts and transactions. It provides methods to read user account data from a specified accounts file and populate a vector of User objects.
#pragma once

#include <string>
#include <vector>
#include "Game.h"
#include "User.h"
using namespace std;

class FileManager {
    public:
        // Constructor initializes the FileManager with the names of the accounts, games, and transaction files for reading and writing
        FileManager(const string& accountsFile, const string& gamesFile, const string& collectionFile, const string& transactionFile);

        // Reads the accounts file and populates the provided vector with User objects
        bool readAccounts(vector<User>& users) const;

        // Reads the games file and populates the provided vector with Game objects
        bool readGames(vector<Game>& games) const;

        // Reads the collection file and populates the provided vector with Game objects
        bool readCollection(vector<Game>& collection) const;

        // Starts a new transaction file for writing
        bool startTransactionFile() const;
        // Writes a create user record to the transaction file
        bool writeCreateRecord(const User& user) const;
        // Writes a delete user record to the transaction file
        bool writeDeleteRecord(const User& user) const;
        // Writes a sell record to the transaction file
        bool writeSellRecord(const Game& game) const;
        // Writes a buy record to the transaction file
        bool writeBuyRecord(const Game& game, const string& buyerUsername) const;
        // Writes a refund record to the transaction file
        bool writeRefundRecord(const string& buyerUsername, const string& sellerUsername, double amount) const;
        // Writes an add credit record to the transaction file
        bool writeAddCreditRecord(const User& user, double amountAdded) const;
        // Writes the end of session marker to the transaction file
        bool writeEndOfSession(const User& user) const;

    private:
        string accountsFile;
        string gamesFile;
        string collectionFile;
        string transactionFile;

        // Writes a record to the transaction file
        bool writeRecord(const string& record) const;
        // Formats a user record for writing to the transaction file
        string formatUserRecord(const string& code, const string& username, const string& accountType, double credit) const;
};