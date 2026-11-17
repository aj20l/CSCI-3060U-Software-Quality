// This header file defines the Game class, which represents a game in the system. Each game has a name, a seller's username, and a price. The class provides methods to access these attributes.
#pragma once

#include <string>
using namespace std;

class Game {
    public:
        // Constructor to initialize a Game object with name, seller's username, and price
        Game(const string& name, const string& sellerUsername, double price) : name(name), sellerUsername(sellerUsername), price(price), listedThisSession(false) {}

        // Getters for the Game attributes
        const string& getName() const { return name; }
        const string& getSellerUsername() const { return sellerUsername; }
        double getPrice() const { return price; }

        void markListedThisSession() { listedThisSession = true; }
        bool wasListedThisSession() const { return listedThisSession; }

    private:
        string name;             
        string sellerUsername;
        double price;
        bool listedThisSession;
};