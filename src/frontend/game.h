#include <string>

class Game {
public:
    Game(std::string gameName, std::string gameSeller, float gamePrice)
        : gameName_(gameName), gameSeller_(gameSeller), gamePrice_(gamePrice) {}

    std::string getName() { return gameName_; }
    std::string getSeller() { return gameSeller_; }
    float getGamePrice() { return gamePrice_; }

private:
    std::string GID_;
    std::string gameName_;
    std::string gameSeller_;
    float gamePrice_;
};