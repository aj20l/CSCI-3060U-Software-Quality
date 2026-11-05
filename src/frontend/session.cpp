#include "session.h"

// Starts the login transaction process
bool Session::login() {
    std::string username;
    std::string password;
    std::cout << "Enter your username: ";
    std::cin >> username;
    std::cout << "Enter your password: ";
    std::cin >> password;
    
    // valdation logic for inputs go here

    // Authenticates login, mapping inputted data to a user if they exist
    currentUser = userManager.authenticateLogin(username, password);
    if(!currentUser) {
        std::cout << "Login failed\n";
        return false;
    }

    sessionActive = true;
    std::cout << "Login successful\n";
    return true;
}

// Starts the logout transaction process
bool Session::logout() {
    if(!userManager.authenticateLogout(currentUser)) {
        std::cout << "Logout failed\n";
        return false;
    }

    currentUser = NULL;
    sessionActive = false;
    std::cout << "Logout successful\n";
    return true;
}

bool Session::isActive() {
    return sessionActive;
}

// Starts the create user transaction process
bool Session::createUser() {
    if(currentUser->getAccountPrivilege() != ADMIN) {
        std::cout << "Cannot delete user, inproper account privilege\n";
        return false;
    }
    
    std::string username;
    std::string password;
    Privilege privilege;
    std::cout << "Enter username of new user: ";
    std::cin >> username;
    std::cout << "Enter password of new user: ";
    std::cin >> password;
    std::cout << "Enter user privilege: ";
    int input;
    std::cin >> input;
    privilege = static_cast<Privilege>(input);

    // Checks the inputs to ensure they fit the constrains

    // Checks if adding a new user fials
    if(!userManager.addUser(username, password, privilege)) {
        std::cout << "Creating new user failed\n";
        return false;
    } 

    std::cout << "Creating new user successful\n";
    // save to daily transaction file
    return true;
}

// Starts the delete user transaction process
bool Session::deleteUser() {
    if(currentUser->getAccountPrivilege() != ADMIN) {
        std::cout << "Cannot delete user, inproper account privilege\n";
        return false;
    }

    std::string username;
    std::cout << "Enter username of user to be deleted: ";
    std::cin >> username;

    // Checks the inputs to ensure they fit the constrains


    // Checks if the user being removed fails
    if(!userManager.removeUser(currentUser->getUsername(), username)) {
        std::cout << "Delete user failed\n";
        return false;
    }

    std::cout << "The user " << username << " has been deleted\n";
    // save to daily transaction file
    return true;
}

// Starts the sell transaction process
bool Session::sell() {
    if(currentUser->getAccountPrivilege() == BUYSTANDARD) {
        std::cout << "Cannot sell, inproper account privilege\n";
        return false;
    }

    if(sessionSellLimited) {
        std::cout << "Cannot sell game, start a new session and try again\n";
        return false;
    }

    std::string gameName;
    float gamePrice;
    std::cout << "Enter name of game: ";
    std::cin >> gameName;
    std::cout << "Enter price of game: ";
    std::cin >> gamePrice;

    // Checks the inputs to ensure they fit the constrains
    if(gameName.length() > 25 || gameName.length() < 3) {
        std::cout << "Invalid game name\n";
        return false;
    }
    if(gamePrice >= 999.99 || gamePrice < 0.0) {
        std::cout << "Invalid game price\n";
        return false;
    }

    // Checks if adding a game listing failed
    if(!gameManager.addGame(gameName, currentUser->getUsername(), gamePrice)) {
        std::cout << "Sell transaction rejected, a game by the seller with the same name already exits\n";
        return false;
    }

    std::cout << "You game has been listed\n";
    sessionSellLimited = true;
    // save to daily transaction file
    return true;
}

// Starts the buy transaction process
bool Session::buy() {
    if(currentUser->getAccountPrivilege() == SELLSTANDARD) {
        std::cout << "Cannot buy game, inproper account privilege\n";
        return false;
    }

    std::string gameName;
    std::string gameSeller;
    std::cout << "Enter name of game: ";
    std::cin >> gameName;
    std::cout << "Enter name of seller: ";
    std::cin >> gameSeller;

    // Checks the inputs to ensure they fit the constrains

    Game* game = gameManager.getGame(gameName, gameSeller);

    // Checks that the game being bought exists, and if their transaction
    // fails
    if(!game) {
        std::cout << "Purchese transaction failed\n";
        return false;
    }
    if(!paymentProcessor.purcheseGame(currentUser, game)){
        std::cout << "Failed to buy game\n";
        return false;
    }
    
    userManager.appendUserFile(currentUser);
    std::cout << "You have bought the game: " << gameName << "\n";
    // save to daily transaction file
    return true;
}

// Starts the refund transaction process
bool Session::refund() {
    if(currentUser->getAccountPrivilege() != ADMIN) {
        std::cout << "Refund transaction rejected, invalid account privilege\n";
        return false;
    }

    std::string buyersName;
    std::string gameName;
    std::string sellerName;
    float refundAmount;
    std::cout << "Enter name of buyer: ";
    std::cin >> buyersName;
    std::cout << "Enter name of game being refunded: ";
    std::cin >> gameName;
    std::cout << "Enter sellers name of the game: ";
    std::cin >> sellerName;
    std::cout << "Enter amount being refunded: ";
    std::cin >> refundAmount;

    // Checks the inputs to ensure they fit the constrains
    if(refundAmount >= 999.99 || refundAmount < 0.0) {
        std::cout << "Refund transaction rejected, invalid refund amount\n";
        return false;
    }

    User* buyer = userManager.getUser(buyersName);
    User* seller = userManager.getUser(sellerName);
    Game* game = gameManager.getGame(gameName, sellerName);
    
    // Checks that the targetd users in the refund are different, and exists
    // with the proper privileges
    if(buyer == seller) {
        std::cout << "Refund transaction rejected, cannot refund self\n";
        return false;
    }
    if(!buyer) {
        std::cout << "Refund transaction rejected, could not find buyer's account\n";
        return false;
    } else if(buyer->getAccountPrivilege() == SELLSTANDARD) {
        std::cout << "Refund transaction rejected, buyer's account privilege cannot buy games to refund\n";
        return false;
    }

    if(!seller) {
        std::cout << "Refund transaction rejected, could not find seller's account\n";
        return false;
    } else if(seller->getAccountPrivilege() == BUYSTANDARD) {
        std::cout << "Refund transaction rejected, seller's account privilege cannot list games to refund\n";
        return false;
    }

    // Checks that games being refunded exists within the system
    if(!game) {
        std::cout << "Refund transaction rejected, could not find game being sold by seller\n";
        return false;
    }

    // Checks if refund being processed fails
    if(!paymentProcessor.refundGame(buyer, seller, game, refundAmount)){
        std::cout << "Refund transaction rejected\n";
        return false;
    }

    userManager.appendUserFile(buyer);
    userManager.appendUserFile(seller);
    std::cout << "Refund transaction accepted, game has been refunded\n";
    // save to daily transaction file
    return true;
}

// Starts the add credit transaction
bool Session::addCredit() {
    if(currentUser->getAccountPrivilege() == ADMIN) {
        std::string targetName;
        float addedAmount;
        std::cout << "Enter name of account credit is being added: ";
        std::cin >> targetName;
        std::cout << "Enter amount being added: ";
        std::cin >> addedAmount;

        // Checks the inputs to ensure they fit the constrains
        if((sessionAddedCreditLimit + addedAmount) > 1000.00) {
            std::cout << "Session limit for adding credit has been reached, start a new sessoin to reset this limit\n";
            return false;
        }

        User* targetAccount = userManager.getUser(targetName);
        
        // Checks if the targets accout exists
        if(!targetAccount) {
            std::cout << "Targeted account credit is being added too could not  be found\n";
            return false;
        }


        // Checks that adding credits to the target is processed
        if(!paymentProcessor.addCredit(targetAccount, addedAmount)) {
            return false;
        }
        sessionAddedCreditLimit += addedAmount;
        userManager.appendUserFile(targetAccount);
    } else {
        float addedAmount;
        std::cout << "Enter amount being added: ";
        std::cin >> addedAmount;

        // Checks the inputs to ensure they fit the constrains
        if((sessionAddedCreditLimit + addedAmount) > 1000.00) {
            return false;
        }

        // Checks that adding credits to the target is processed
        if(!paymentProcessor.addCredit(currentUser, addedAmount)) {
            return false;
        }
        sessionAddedCreditLimit += addedAmount;
        userManager.appendUserFile(currentUser);
    }

    std::cout << "Credit has been added to account\n";
    // save to daily transaction file
    return true;
}

// Starts the transaction process listing all active accounts
bool Session::listActiveAccounts() {
    if(currentUser->getAccountPrivilege() != ADMIN) {
        std::cout << "Cannot refund game, inproper account privilege\n";
        return false;
    }
    std::cout << "All active accounts\n";
    for(User* user : userManager.getUserList()) {
        if(user->getActivity() == true) {
            std::cout << user->getUsername();
        }
    }
    return true;
}
