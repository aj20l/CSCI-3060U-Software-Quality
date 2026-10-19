#include "payment_processor.h"

bool PaymentProcessor::purcheseGame(User* user, Game* game) {
    if(user->hasGame(game->getName(), game->getSeller())) { // change this later
        std::cout << "Current account already has game\n";
        return false;
    }

    if(user->getBalance() < game->getGamePrice()) {
        std::cout << "Invalid balance to buy game\n";
        return false;
    }
    
    user->appendGame(game);
    float newBalance = user->getBalance() - game->getGamePrice();
    user->setBalance(newBalance);
    return true;
}

bool PaymentProcessor::refundGame(User* buyer, User* seller, Game* game, float refundAmount) {
    if(buyer->hasGame(game->getName(), game->getSeller())) {
        std::cout << "Could not refund game, buyer doesn't have the game\n";
        return false;
    }

    if(seller->getBalance() < refundAmount) {
        std::cout << "Could not refund game, seller doesn't have the balance  for the refund\n";
        return false;
    }
}

bool PaymentProcessor::addCredit(User* user, float addAmount) {
    float newBalance = user->getBalance() + addAmount;
    user->setBalance(newBalance);
    return true;
}