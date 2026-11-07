#include "payment_processor.h"

// Processes a game purchese
bool PaymentProcessor::purcheseGame(User* user, Game* game) {
    // Checks that the user doesn't already have the game being purchesed
    if(user->hasGame(game->getName(), game->getSeller())) {
        std::cout << "Current account already has game\n";
        return false;
    }

    // Checks that the user has sufficient credits for purchese
    if(user->getBalance() < game->getGamePrice()) {
        std::cout << "Invalid balance to buy game\n";
        return false;
    }
    
    user->appendGame(game);
    float newBalance = user->getBalance() - game->getGamePrice();
    user->setBalance(newBalance);
    return true;
}

// Processes a refund for a game between two users
bool PaymentProcessor::refundGame(User* buyer, User* seller, Game* game, float refundAmount) {
    // Checks the game being refunded exists in the buyers game list
    if(!buyer->hasGame(game->getName(), game->getSeller())) {
        std::cout << "Buyer doesn't have the game\n";
        return false;
    }

    // Checks that the user and seller have sufficient credits
    // for the refund transaction
    float newBalanceBuyer = buyer->getBalance() + refundAmount;
    float newBalanceSeller = seller->getBalance() - refundAmount;
    if(newBalanceBuyer > 999999) {
        std::cout << "Buyer's balance limit surpasses\n";
        return false;
    }
    if(newBalanceSeller < 0) {
        std::cout << "Seller doesn't have the balance for the refund\n";
        return false;
    }

    buyer->setBalance(newBalanceBuyer);
    seller->setBalance(newBalanceSeller);
    buyer->removeGame(game);
    return true;
}

// Processes adding credits to a user
bool PaymentProcessor::addCredit(User* user, float addAmount) {
    float newBalance = user->getBalance() + addAmount;
    user->setBalance(newBalance);
    return true;
}