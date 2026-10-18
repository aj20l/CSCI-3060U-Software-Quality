#include <string>
#include <optional>
using namespace std;
class Game {
    public:
        float getGamePrice() { return gamePrice; }

    private:
        std::string GID;
        std::string gameName;
        std::string gameSeller;
        float gamePrice;
};

class GameManager {
    public:
        GameManager() = default;

        bool createGame(std::string gameName, float gamePrice, std::string gameSeller) {
		return false;
        }
        bool deleteGame() {return false;}
        std::optional<Game> getGame(std::string gameName, std::string gameSeller) {
            // add some kind of search function to find the user
            return std::nullopt;
        }

    private:
        Game games[10];

        void loadGamesFile() {}
};
