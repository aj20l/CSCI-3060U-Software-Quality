#include "session.h"

bool Session::login() {
    std::string username;
    std::string password;
    std::cout << "Enter your username: ";
    std::cin >> username;
    std::cout << "Enter your password: ";
    std::cin >> password;
    
    // valdation logic for inputs go here

    currentUser = userManager.authenticateLogin(username, password);
    if(!currentUser) {
        std::cout << "Login failed\n";
        return false;
    }

    sessionActive = true;
    std::cout << "Login successful\n";
    return true;
}

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

    // logic check for username and password to see if they're valid

    if(!userManager.addUser(username, password, privilege)) {
        std::cout << "Creating new user failed\n";
        return false;
    } 

    std::cout << "Creating new user successful\n";
    // save to daily transaction file
    return true;
}

bool Session::deleteUser() {
    if(currentUser->getAccountPrivilege() != ADMIN) {
        std::cout << "Cannot delete user, inproper account privilege\n";
        return false;
    }

    std::string username;
    std::cout << "Enter username of user to be deleted: ";
    std::cin >> username;

        // logic check for username to see if they're valid

    if(!userManager.removeUser(currentUser->getUsername(), username)) {
        std::cout << "Delete user failed\n";
        return false;
    }

    std::cout << "The user " << username << " has been deleted\n";
    // save to daily transaction file
    return true;
}

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

    // validation logic goes here for game name and price
    if(gamePrice >= 999.99 || gamePrice < 0.0) {
        std::cout << "Inproper game price\n";
        return false;
    }

    if(!gameManager.addGame(gameName, currentUser->getUsername(), gamePrice)) {
        std::cout << "An error has occured trying to create game listing\n";
        return false;
    }

    std::cout << "You game has been listed\n";
    sessionSellLimited = true;
    // save to daily transaction file
    return true;
}

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

    // logic to validate game name and seller name

    Game* game = gameManager.getGame(gameName, gameSeller);
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

bool Session::refund() {
    if(currentUser->getAccountPrivilege() != SELLSTANDARD) {
        std::cout << "Cannot refund game, inproper account privilege\n";
        return false;
    }

    std::string buyersName;
    std::string gameName;
    float refundAmount;
    std::cout << "Enter name of buyer: ";
    std::cin >> buyersName;
    std::cout << "Enter name of game being refunded: ";
    std::cin >> gameName;
    std::cout << "Enter amount being refunded: ";
    std::cin >> refundAmount;

    // validation logic for buyers name, game name, and refund amount

    User* buyer = userManager.getUser(buyersName);
    Game* game = gameManager.getGame(gameName, currentUser->getUsername());
    
    if(!buyer) {
        std::cout << "Could not find buyer's account for refund transaction\n";
        return false;
    }

    if(!game) {
        std::cout << "Could not find game being sold by seller\n";
        return false;
    }

    // handles rest of the logic in transaction processor
    if(!paymentProcessor.refundGame(buyer, currentUser, game, refundAmount)){
        std::cout << "Refund transaction failed\n";
        return false;
    }

    userManager.appendUserFile(currentUser);
    // save to daily transaction file
    return true;
}

bool Session::addCredit() { // will optimize this method later, got lazy
    if(currentUser->getAccountPrivilege() == ADMIN) {
        std::string targetName;
        float addedAmount;
        std::cout << "Enter name of account credit is being added: ";
        std::cin >> targetName;
        std::cout << "Enter amount being added: ";
        std::cin >> addedAmount;

        // validation logic for target name and amount

        User* targetAccount = userManager.getUser(targetName);

        if(!targetAccount) {
            std::cout << "Targeted account credit is being added too could not  be found\n";
            return false;
        }

        if((sessionAddedCreditLimit + addedAmount) > 1000.00) {
            std::cout << "Session limit for adding credit has been reached, start a new sessoin to reset this limit\n";
            return false;
        }

        sessionAddedCreditLimit += addedAmount;
        if(!paymentProcessor.addCredit(targetAccount, addedAmount)) {
            return false;
        }

    } else {
        float addedAmount;
        std::cout << "Enter amount being added: ";
        std::cin >> addedAmount;

        // validation logic for the amount

        if((sessionAddedCreditLimit + addedAmount) > 1000.00) {
            return false;
        }

        sessionAddedCreditLimit += addedAmount;
        if(!paymentProcessor.addCredit(currentUser, addedAmount)) {
            return false;
        }
    }

    std::cout << "Credit has been added to account\n";
    // save to daily transaction file
    return true;
}

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
