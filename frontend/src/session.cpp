#include "managers.cpp"

class Session {
    public:
        Session() = default;

        bool login() {
            std::string username;
            std::string password;
            std::cout << "Enter your username: ";
            std::cin >> username;
            std::cout << "Enter your password: ";
            std::cin >> password;

            currentUser = userManager.getUser(username);
            if(!currentUser) {
            	std::cout<<"User does not exist";
                return false;
            }

            if(!currentUser->checkPassword(password)) {
                return false;
            }

            if(currentUser->isActive()) {
                std::cout << "The current user already has a session\n";
                return false;
            }

            currentUser->setUserActive();
            std::cout << "Login successful\n";
            return true;
        }
        bool logout() {
            currentUser->setUserActive();
            
            return true;
        }
        bool isActive() {
            return currentUser->isActive();
        }
        bool createUser() {
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

            if(!userManager.createUser(username, password, privilege)) {
                std::cout << "An error occured while trying to create new user\n";
                return false;
            } 

            std::cout << "New user has been created\n";
            // save to daily transaction file
            return true;
        }
        bool deleteUser() {
            if(currentUser->getAccountPrivilege() != ADMIN) {
                std::cout << "Cannot delete user, inproper account privilege\n";
            }

            std::string username;
            std::cout << "Enter username of user to be deleted: ";
            std::cin >> username;

            if(currentUser->getUsername() == username) {
                std::cout << "Cannot delete current account being used\n";
                return false;
            }

            if(!userManager.deleteUser(username)) {
                std::cout << "An error has occured trying to deleted user\n";
                return false;
            }

            std::cout << "The user " << username << " has been deleted\n";
            // save to daily transaction file
            return true;
        }
        bool sell() {
            if(currentUser->getAccountPrivilege() != (ADMIN || FULLSTANDARD || SELLSTANDARD)) {
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

            if(gamePrice >= 999.99 || gamePrice < 0.0) {
                std::cout << "Inproper game price\n";
                return false;
            }
            if(!gameManager.createGame(gameName, gamePrice, currentUser->getUsername())) {
                std::cout << "An error has occured trying to create game listing\n";
                return false;
            }
            std::cout << "You game has been listed\n";
            sessionSellLimited = true;
            // save to daily transaction file
            return true;
        }
        bool buy() {
            if(currentUser->getAccountPrivilege() != (ADMIN || FULLSTANDARD || BUYSTANDARD)) {
                std::cout << "Cannot buy game, inproper account privilege\n";
                return false;
            }

            std::string gameName;
            std::string gameSeller;
            std::cout << "Enter name of game: ";
            std::cin >> gameName;
            std::cout << "Enter name of seller: ";
            std::cin >> gameSeller;
            std::optional<Game> game = gameManager.getGame(gameName, gameSeller);

            if(!game) {
                std::cout << "Could not find game\n";
                return false;
            }

            if(currentUser->getBalance() < game->getGamePrice()) {
                std::cout << "Invalid balance to buy game\n";
                return false;
            }

            if(currentUser->hasGame(game.value())) { // change this later
                std::cout << "Current account already has game\n";
                return false;
            }

            std::cout << "You have bought the game: " << gameName << "\n";
            // save to daily transaction file
            return true;
        }
        bool refund() {
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

            std::optional<User> buyer = userManager.getUser(buyersName);
            std::optional<Game> game = gameManager.getGame(gameName, currentUser->getUsername());
            
            if(!buyer) {
                std::cout << "Could not find buyer's account for refund transaction\n";
                return false;
            }

            if(!game) {
                std::cout << "Could not find game being sold by seller\n";
                return false;
            }
            
            if(!buyer->hasGame(game.value())) {
                std::cout << "Could not refund game, buyer doesn't have the game\n";
                return false;
            }

            if(currentUser->getBalance() < refundAmount) {
                std::cout << "Could not refund game, seller doesn't have the balance  for the refund\n";
                return false;
            }

            buyer->setBalance(refundAmount);
            currentUser->setBalance(-refundAmount);
            // save to daily transaction file
            return true;
        }
        bool addCredit() { // will optimize this method later, got lazy
            if(currentUser->getAccountPrivilege() == ADMIN) {
                std::string targetName;
                float addedAmount;
                std::cout << "Enter name of account credit is being added: ";
                std::cin >> targetName;
                std::cout << "Enter amount being added: ";
                std::cin >> addedAmount;

                std::optional<User> buyer = userManager.getUser(targetName);

                if(!buyer) {
                    std::cout << "Targeted account credit is being added too could not  be found\n";
                    return false;
                }

                if((sessionAddedCreditLimit + addedAmount) > 1000.00) {
                    std::cout << "Session limit for adding credit has been reached, start a new sessoin to reset this limit\n";
                    return false;
                }

                sessionAddedCreditLimit += addedAmount;
                buyer->setBalance(addedAmount);

            } else {
                float addedAmount;
                std::cout << "Enter amount being added: ";
                std::cin >> addedAmount;

                if((sessionAddedCreditLimit + addedAmount) > 1000.00) {
                    return false;
                }

                sessionAddedCreditLimit += addedAmount;
                currentUser->setBalance(addedAmount);
            }

            // save to daily transaction file
            std::cout << "Credit has been added to account\n";
            return true;
        }

    private:
        UserManager userManager;  
        GameManager gameManager;

        std::optional<User> currentUser;    
        bool sessionSellLimited = false;
        float sessionAddedCreditLimit = 0;

};