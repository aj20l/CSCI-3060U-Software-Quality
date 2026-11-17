// This header file defines the GameStore class, which manages a collection of games available for sale. It provides methods to load games from a file, find a specific game, add new games, and retrieve the list of available games.
#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include "FileManager.h"
#include "Game.h"
using namespace std;

class GameStore {
    public:
        // Asks filemanager to read the games file and keep the result
        bool loadGames(const FileManager& files) { return files.readGames(availableGames) && files.readCollection(collections); }

        // Checks if a game with the given name exists in the available games
        bool gameNameExists(const string& name) const {
            return any_of(availableGames.begin(), availableGames.end(), [&](const Game& game) { return game.getName() == name; });
        }

        // Finds a game by name and seller's username and returns a pointer to the Game object, or nullptr if not found
        const Game* findGame(const string& name, const string& sellerUsername) const {
            for (const Game& game : availableGames) {
                if (game.getName() == name && game.getSellerUsername() == sellerUsername) {
                    return &game;
                }
            }
            return nullptr;
        }

        // Puts a new game up for sale.
        void addGame(const Game& game) { availableGames.push_back(game); }

        // Removes all games from the available games that are sold by the specified seller
        void removeGamesBySeller(const string& username) {
            availableGames.erase(
                remove_if(availableGames.begin(), availableGames.end(), [&](const Game& game) { return game.getSellerUsername() == username; }),
                availableGames.end()
            );
        }

        // Checks if a user is currently selling any games in the available games
        bool sellsGames(const string& username) const {
            return any_of(availableGames.begin(), availableGames.end(),
                        [&](const Game& game) { return game.getSellerUsername() == username; });
        }

        // Checks if a user owns a specific game in their collection
        bool ownsGame(const string& username, const string& gameName) const {
            return any_of(collections.begin(), collections.end(), [&](const Game& entry) {
                return entry.getName() == gameName && entry.getSellerUsername() == username;
            });
        }

        // Adds a game to a user's collection
        void addToCollection(const string& username, const string& gameName) {
            collections.push_back(Game(gameName, username, 0.0));
        }

        // Returns a reference to the vector of all available games
        const vector<Game>& getGames() const { return availableGames; }

        // Returns a vector of game names owned by the specified user
        vector<string> getGamesOwnedBy(const string& username) const {
            vector<string> names;
            for (const Game& entry : collections) {
                // If the entry's seller username matches the specified username, add the game name to the list of names
                if (entry.getSellerUsername() == username) names.push_back(entry.getName());
            }
            return names;
        }

    private:
        vector<Game> availableGames;
        vector<Game> collections;
};