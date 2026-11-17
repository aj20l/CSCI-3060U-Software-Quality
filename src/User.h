// This header file defines the User class, which represents a user in the system with attributes such as username, account type, and credit. It provides methods to access these attributes and determine if the user is an admin.
#pragma once

#include <string>
using namespace std;

// The most credit one account may hold
const double MAX_CREDIT = 999999.00;

class User {
    public:
        // Constructor to initialize a User object with username, account type, and credit
        User(const string& username, const string& accountType, double credit): username(username), accountType(accountType), credit(credit) {}

        // Getters for the User attributes
        const string& getUsername() const { return username; }
        const string& getAccountType() const { return accountType; }
        double getCredit() const { return credit; }

        // Methods to check the user's permissions based on account type
        bool isAdmin() const { return accountType == "AA"; }
        bool canSell() const { return accountType != "BS"; }
        bool canBuy() const { return accountType != "SS"; }

        // Methods to manage the user's credit
        bool hasCredit(double amount) const { return credit + 0.001 >= amount; }
        bool canReceive(double amount) const { return credit + amount <= MAX_CREDIT + 0.001; }
        void addCredit(double amount) { credit += amount; }
        void deductCredit(double amount) { credit -= amount; }

        // Get the human-readable name of the account type
        string getTypeName() const {
            if (accountType == "AA") return "admin";
            if (accountType == "FS") return "full-standard";
            if (accountType == "BS") return "buy-standard";
            if (accountType == "SS") return "sell-standard";
            return "unknown";
        }

        // Static method to convert a human-readable account type name to its corresponding code
        static string codeFromTypeName(const string& typeName) {
            if (typeName == "admin") return "AA";
            if (typeName == "full-standard") return "FS";
            if (typeName == "buy-standard") return "BS";
            if (typeName == "sell-standard") return "SS";
            return "";
        }

    private:
        string username;
        string accountType;
        double credit;
};