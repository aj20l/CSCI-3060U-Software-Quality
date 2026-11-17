// This source file implements the FileManager class, which is responsible for managing file operations related to user accounts and transactions. It provides methods to read user account data from a specified accounts file and populate a vector of User objects.
#include "FileManager.h"
#include "TextUtils.h"

#include <cstdlib>
#include <fstream>
using namespace std;

namespace {
    const size_t USERNAME_WIDTH = 15;
    const size_t TYPE_WIDTH = 2;
    const size_t CREDIT_WIDTH = 9;

    const size_t GAME_NAME_WIDTH = 26;
    const size_t SELLER_USERNAME_WIDTH = 15;
    const size_t PRICE_WIDTH = 6;

    // One account line = username + space + type + space + credit
    const size_t ACCOUNT_LINE_WIDTH = USERNAME_WIDTH + 1 + TYPE_WIDTH + 1 + CREDIT_WIDTH;

    // One game line = name + space + seller + space + price
    const size_t GAME_LINE_WIDTH = GAME_NAME_WIDTH + 1 + SELLER_USERNAME_WIDTH + 1 + PRICE_WIDTH;

    // One collection line = name + space + seller
    const size_t MIN_COLLECTION_LINE_WIDTH = 1 + 1 + USERNAME_WIDTH;

    const string CODE_END_OF_SESSION = "00";
    const string CODE_CREATE = "01";
    const string CODE_DELETE = "02";
    const string CODE_SELL = "03";
    const string CODE_BUY = "04";
    const string CODE_REFUND = "05";
    const string CODE_ADD_CREDIT = "06";
    const string END_MARKER = "END";
}

// Constructor initializes the FileManager with the names of the accounts, games, and transaction files for reading and writing
FileManager::FileManager(const string& accountsFile, const string& gamesFile, const string& collectionFile, const string& transactionFile) : accountsFile(accountsFile), gamesFile(gamesFile), collectionFile(collectionFile), transactionFile(transactionFile) {}

bool FileManager::readAccounts(vector<User>& users) const {
    // Try to open the accounts file for reading
    ifstream file(accountsFile);
    if (!file) return false;

    users.clear();
    string line;
    // Read the file one line at a time until there are no more lines
    while (getline(file, line)) {
        // The END line means "no more accounts"
        if (trim(line) == END_MARKER) break;
        // Skip lines that are too short to be a real account line
        if (line.size() < ACCOUNT_LINE_WIDTH) continue;

        // Cut the line into its three fields
        string username = trim(line.substr(0, USERNAME_WIDTH));
        string accountType = line.substr(USERNAME_WIDTH + 1, TYPE_WIDTH);
        double credit = atof(line.substr(USERNAME_WIDTH + 1 + TYPE_WIDTH + 1, CREDIT_WIDTH).c_str());

        // Build a User from the three fields and add it to the list
        users.push_back(User(username, accountType, credit));
    }
    return true;
}


bool FileManager::readGames(vector<Game>& games) const {
    // Try to open the games file for reading
    ifstream file(gamesFile);
    if (!file) return false;

    games.clear();
    string line;
    // Read the file one line at a time until there are no more lines
    while (getline(file, line)) {
        // The END line means "no more games"
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (trim(line) == END_MARKER) break;
        if (line.size() < GAME_LINE_WIDTH) continue;

        // Cut the line into its three fields
        size_t priceStart = line.size() - PRICE_WIDTH;
        size_t sellerStart = priceStart - 1 - SELLER_USERNAME_WIDTH;
        string name = trim(line.substr(0, sellerStart));
        string sellerUsername = trim(line.substr(sellerStart, SELLER_USERNAME_WIDTH));
        double price = atof(line.substr(priceStart).c_str());

        // Build a Game from the three fields and add it to the list
        games.push_back(Game(name, sellerUsername, price));
    }
    return true;
}


bool FileManager::readCollection(vector<Game>& collection) const {
    // Try to open the collection file for reading.
    ifstream file(collectionFile);
    if (!file) return false;

    collection.clear();
    string line;
    while (getline(file, line)) {
        // The END line means "no more collection entries"
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (trim(line) == END_MARKER) break;
        if (line.size() < MIN_COLLECTION_LINE_WIDTH) continue;

        // Cut the line into its two fields: name and owner. The owner is always the last 15 characters of the line, and the name is everything before that.
        size_t ownerStart = line.size() - USERNAME_WIDTH;
        string name = trim(line.substr(0, ownerStart));
        string owner = trim(line.substr(ownerStart));

        // Build a Game from the two fields and add it to the collection. The price is not stored in the collection file, so we set it to 0.0.
        collection.push_back(Game(name, owner, 0.0));
    }
    return true;
}


// Starts a new transaction file for writing
bool FileManager::startTransactionFile() const {
    // Opening a file for writing empties it, so each session starts with a fresh file.
    ofstream file(transactionFile);
    return static_cast<bool>(file);
}

// Writes a record to the transaction file
bool FileManager::writeCreateRecord(const User& user) const {
    return writeRecord(formatUserRecord(CODE_CREATE, user.getUsername(), user.getAccountType(), user.getCredit()));
}

// Writes a delete user record to the transaction file
bool FileManager::writeDeleteRecord(const User& user) const {
    return writeRecord(formatUserRecord(CODE_DELETE, user.getUsername(), user.getAccountType(), user.getCredit()));
}

// Writes a sell record to the transaction file
bool FileManager::writeSellRecord(const Game& game) const {
    // The game name, seller and price, each padded to its fixed width.
    return writeRecord(CODE_SELL + " " + padRight(game.getName(), GAME_NAME_WIDTH) + " " +
                       padRight(game.getSellerUsername(), USERNAME_WIDTH) + " " +
                       formatMoney(game.getPrice(), PRICE_WIDTH)
    );
}

// Writes a buy record to the transaction file
bool FileManager::writeBuyRecord(const Game& game, const string& buyerUsername) const {
    return writeRecord(CODE_BUY + " " + padRight(game.getName(), GAME_NAME_WIDTH) + " " +
                       padRight(game.getSellerUsername(), USERNAME_WIDTH) + " " +
                       padRight(buyerUsername, USERNAME_WIDTH) + " " +
                       formatMoney(game.getPrice(), PRICE_WIDTH)
    );
}

// Writes a refund record to the transaction file
bool FileManager::writeRefundRecord(const string& buyerUsername, const string& sellerUsername, double amount) const {
    return writeRecord(CODE_REFUND + " " + padRight(buyerUsername, USERNAME_WIDTH) + " " +
                       padRight(sellerUsername, USERNAME_WIDTH) + " " +
                       formatMoney(amount, CREDIT_WIDTH)
    );
}

// Writes an add credit record to the transaction file
bool FileManager::writeAddCreditRecord(const User& user, double amountAdded) const {
    return writeRecord(formatUserRecord(CODE_ADD_CREDIT, user.getUsername(), user.getAccountType(), amountAdded));
}

// Writes the end of session marker to the transaction file
bool FileManager::writeEndOfSession(const User& user) const {
    return writeRecord(formatUserRecord(CODE_END_OF_SESSION, user.getUsername(), user.getAccountType(), user.getCredit()));
}


// Formats a user record for writing to the transaction file
bool FileManager::writeRecord(const string& record) const {
    // Open the transaction file for appending. If it doesn't exist, create it.
    ofstream file(transactionFile, ios::app);
    if (!file) return false;

    file << record << "\n";
    return static_cast<bool>(file);
}


// Formats a user record for writing to the transaction file
string FileManager::formatUserRecord(const string& code, const string& username, const string& accountType, double credit) const {
    return code + " " + padRight(username, USERNAME_WIDTH) + " " + padRight(accountType, TYPE_WIDTH) + " " + formatMoney(credit, CREDIT_WIDTH);
}

