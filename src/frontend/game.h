#pragma once
#include <string>

class Game {
public:
    Game(int gid, std::string gameName, std::string gameSeller, float gamePrice)
        : GID_(gid), gameName_(gameName), gameSeller_(gameSeller), gamePrice_(gamePrice) {}

    int getGID() { return GID_; }
    std::string getName() { return gameName_; }
    std::string getSeller() { return gameSeller_; }
    float getGamePrice() { return gamePrice_; }

private:
    int GID_;
    std::string gameName_;
    std::string gameSeller_;
    float gamePrice_;
};