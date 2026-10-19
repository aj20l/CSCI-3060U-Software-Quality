#include <string>
#include <vector>
//#include "game.cpp"
#include "user_manager.cpp"

class GameManager {
    public:
        GameManager() = default;

        bool addGame(std::string gameName, std::string gameSeller, float gamePrice) {
            Game* game = getGame(gameName, gameSeller);
            if(game) {
                std::cout << "A game by the seller with the same name already exits\n";
                return false;
            }

            Game* newGame = new Game(gameName, gameSeller, gamePrice);
            games.push_back(newGame);
            return true;
        }
        bool removeGame(std::string gameName, std::string gameSeller) { 
            Game* game = getGame(gameName, gameSeller);
            if(!game) {
                std::cout << "Game being deleted doesn't exist\n";
                return false;
            }
            // logic to remove game from listing
            return true;
        }
        std::vector<Game*> getGames() { return games; }
        Game* getGame(std::string gameName, std::string gameSeller) {
            for(Game* game : games) {
                if(game->getName() == gameName && game->getSeller() == gameSeller) {
                    return game;
                }
            }
            std::cout << "Could not find game\n";
            return NULL;
        }

    private:
        std::vector<Game*> games;

        void loadGamesFile() {}
};
