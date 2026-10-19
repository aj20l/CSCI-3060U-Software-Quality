#include <iostream>
#include <string>
#include "user.h"
#include "game.h"

class PaymentProcessor {
    public:
        bool purcheseGame(User* user, Game* game);
        bool refundGame(User* buyer, User* seller, Game* game, float refundAmount);
        bool addCredit(User* user, float addAmount);

};