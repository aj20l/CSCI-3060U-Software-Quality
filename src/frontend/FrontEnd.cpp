
// This file implements the FrontEnd class, which manages user interactions,
// including login, logout, and account management.
// It uses FileManager for file operations, SessionManager for session handling,
// UserStore for managing user data, and GameStore for managing games.

#include "FrontEnd.h"
#include "TextUtils.h"
#include <set>
#include <iomanip>
using namespace std;

namespace {
    const size_t MAX_USERNAME_LENGTH = 15;
    const size_t MAX_GAME_NAME_LENGTH = 25;
    const double MAX_PRICE = 999.99;
    const double MAX_CREDIT_PER_SESSION = 1000.00;
}

// Constructor initializes the FrontEnd with input/output streams and a FileManager for file operations
FrontEnd::FrontEnd(istream& input, ostream& output, const FileManager& files) : input(input), output(output), files(files) {}


// Runs the front-end program
void FrontEnd::run() {
    displayMessage("Games Distribution Front End. Enter a transaction code (type login to begin).");

    string command;
    // Keep reading commands until the input ends
    while (true) {
        output << "Enter transaction: " << flush;

        if (!getline(input, command)) {
            break;
        }

        if (!command.empty() && command.back() == '\r') {
            command.pop_back();
        }

        handleCommand(command);
    }
}


// Decides what to do with command typed by the user
void FrontEnd::handleCommand(const string& command) {
    if (command == "login") {
        handleLogin();
    } else if (command == "logout") {
        handleLogout();
    } else if (!session.isLoggedIn()) {
        displayError("You must log in first.");
    } else if (!isPermitted(command)) {
        displayError("Your account type is not permitted to do that.");
    } else if (command == "listaccounts") {
        handleListAccounts();
    } else if (command == "listgames") {
        handleListGames();
    } else if (command == "listcollections") {
        handleListCollections();
    } else if (command == "search") {
        handleSearch();
    } else if (command == "create") {
        handleCreate();
    } else if (command == "delete") {
        handleDelete();
    } else if (command == "sell") {
        handleSell();
    } else if (command == "buy") {
        handleBuy();
    } else if (command == "refund") {
        handleRefund();
    } else if (command == "addcredit") {
        handleAddCredit();
    } else {
        displayError("Unknown transaction code.");
    }
}


// Handles the login process
void FrontEnd::handleLogin() {
    // Cannot login if already logged in
    if (session.isLoggedIn()) {
        displayError("You are already logged in.");
        return;
    }

    // Load the users from the accounts file
    if (!users.loadUsers(files)) {
        displayError("The user accounts file could not be read.");
        return;
    }

    // Load the games from the games file
    if (!games.loadGames(files)) {
        displayError("The available games or game collection file could not be read.");
        return;
    }

    // Start a new transaction file for writing
    if (!files.startTransactionFile()) {
        displayError("The daily transaction file could not be created.");
        return;
    }

    // Prompt the user for their username
    string username;
    if (!promptForInput("Enter username: ", username)) return;

    // Look up the username in the list of users
    const User* user = users.findUser(username);
    if (user == nullptr) {
        displayError("Unknown username.");
        return;
    }

    // Start a session for the user and display a success message
    session.startSession(*user);
    displayMessage("Logged in as " + user->getUsername() + ".");
}


// Handles the logout process
void FrontEnd::handleLogout() {
    // Logout is only allowed if a user is currently logged in
    if (!session.isLoggedIn()) {
        displayError("You must log in first.");
        return;
    }

    // Write end of session marker to the transaction file and end the session
    const User* currentUser = users.findUser(session.getCurrentUser().getUsername());
    if (!files.writeEndOfSession(*currentUser)) {
        displayError("The daily transaction file could not be written.");
    }

    session.endSession();
    displayMessage("Logged out.");
}


// Lists all user accounts (admin only)
void FrontEnd::handleListAccounts() {
    // Display all user accounts
    displayMessage("Active accounts:");
    output << left << setw(16) << "Username"
           << setw(15) << "Type" << "Credit\n";

    for (const User& user : users.getUsers()) {
        output << left << setw(16) << user.getUsername()
               << setw(15) << user.getTypeName()
               << fixed << setprecision(2) << user.getCredit() << "\n";
    }

    displayMessage(to_string(users.getUsers().size()) + " account(s).");
}


// Lists all available games
void FrontEnd::handleListGames() {
    // Display all games loaded from the available games file
    displayMessage("Available games:");
    output << left << setw(27) << "Game"
           << setw(16) << "Seller" << "Price\n";

    for (const Game& game : games.getGames()) {
        output << left << setw(27) << game.getName()
               << setw(16) << game.getSellerUsername()
               << fixed << setprecision(2) << game.getPrice() << "\n";
    }

    displayMessage(to_string(games.getGames().size()) + " game(s).");
}


// Lists all games in the current user's collection
void FrontEnd::handleListCollections() {
    string username = session.getCurrentUser().getUsername();

    // Display all games in the user's collection
    vector<string> ownedGames = games.getGamesOwnedBy(username);

    // Display the user's game collection
    displayMessage("Game collection of " + username + ":");
    for (const string& gameName : ownedGames) {
        output << "  " << gameName << "\n";
    }
    displayMessage(to_string(ownedGames.size()) + " game(s).");
}


// Handles the search for games and users based on a query string
void FrontEnd::handleSearch() {
    string query;
    if (!promptForInput("Enter search text: ", query)) return;
    query = trim(query);

    // If the query is empty, display an error message and return
    if (query.empty()) {
        displayError("Please enter some text to search for.");
        return;
    }

    // Print each game whose name contains the text
    displayMessage("Games matching \"" + query + "\":");
    int gameCount = 0;
    for (const Game& game : games.getGames()) {
        if (containsText(game.getName(), query)) {
            output << "  " << left << setw(27) << game.getName() << setw(16)
                   << game.getSellerUsername() << fixed << setprecision(2) << game.getPrice()
                   << "\n";
            gameCount++;
        }
    }
    displayMessage(to_string(gameCount) + " game(s).");

    // Print each user whose username contains the text, excluding hidden admin users
    displayMessage("Users matching \"" + query + "\":");
    int userCount = 0;
    for (const User& user : users.getUsers()) {
        bool nameMatches = containsText(user.getUsername(), query);
        bool hiddenFromThisUser =
            !session.isAdmin() && user.isAdmin() && !games.sellsGames(user.getUsername());
        if (nameMatches && !hiddenFromThisUser) {
            output << "  " << user.getUsername() << "\n";
            userCount++;
        }
    }
    displayMessage(to_string(userCount) + " user(s).");
}


// Handles the creation of a new user account (admin only)
void FrontEnd::handleCreate() {
    string name;
    string typeName;
    if (!promptForInput("Enter new username: ", name)) return;
    if (!promptForInput("Enter account type (admin, full-standard, buy-standard, sell-standard): ", typeName)) {
        return;
    }
    name = trim(name);
    typeName = trim(typeName);

    // The username must be non-empty, not "END", and not already taken
    if (name.empty() || name == "END") {
        displayError("That is not a valid username.");
        return;
    }
    if (name.size() > MAX_USERNAME_LENGTH) {
        displayError("Usernames can be at most 15 characters long.");
        return;
    }
    if (users.findUser(name) != nullptr) {
        displayError("That username is already taken.");
        return;
    }

    // The account type must be one of the four valid types.
    string accountType = User::codeFromTypeName(typeName);
    if (accountType.empty()) {
        displayError("Unknown account type.");
        return;
    }

    // Create the new user with 0.0 credit, add it to the user store, and write a create record to the transaction file
    User newUser(name, accountType, 0.0);
    users.addUser(newUser);
    files.writeCreateRecord(newUser);
    displayMessage("User " + name + " created.");
}


// Handles the deletion of a user account (admin only)
void FrontEnd::handleDelete() {
    string name;
    if (!promptForInput("Enter username to delete: ", name)) return;
    name = trim(name);

    // The username must exist in the user store
    const User* userToDelete = users.findUser(name);
    if (userToDelete == nullptr) {
        displayError("No such user.");
        return;
    }
    // The admin cannot delete their own account while logged in
    if (name == session.getCurrentUser().getUsername()) {
        displayError("You cannot delete the account you are logged in with.");
        return;
    }

    // Write a delete record to the transaction file, remove all games sold by that user, and remove the user from the user store
    files.writeDeleteRecord(*userToDelete);
    games.removeGamesBySeller(name);
    users.removeUser(name);
    displayMessage("User " + name + " deleted.");
}


// Handles the process of putting a game up for sale
void FrontEnd::handleSell() {
    string name;
    string priceText;
    if (!promptForInput("Enter game name: ", name)) return;
    if (!promptForInput("Enter price in dollars (e.g. 15.00): ", priceText)) return;
    name = trim(name);

    // The game name must be non-empty, not "END", not too long, and not already taken
    if (name.empty() || name == "END") {
        displayError("That is not a valid game name.");
        return;
    }
    if (name.size() > MAX_GAME_NAME_LENGTH) {
        displayError("Game names can be at most 25 characters long.");
        return;
    }
    if (games.gameNameExists(name)) {
        displayError("A game with that name already exists.");
        return;
    }

    // The price must be a valid number with at most 2 decimal places and not exceed the maximum allowed price
    double price;
    if (!parseMoney(priceText, price)) {
        displayError("That is not a valid price.");
        return;
    }
    if (price > MAX_PRICE + 0.001) {
        displayError("The price cannot be more than 999.99.");
        return;
    }

    // Create a new Game object with the current user's username as the seller, mark it as listed this session, add it to the game store, and write a sell record to the transaction file
    Game newGame(name, session.getCurrentUser().getUsername(), price);
    newGame.markListedThisSession();
    games.addGame(newGame);
    files.writeSellRecord(newGame);
    displayMessage(name + " is now for sale.");
}


// Handles the process of buying a game
void FrontEnd::handleBuy() {
    string name;
    string sellerName;
    if (!promptForInput("Enter game name: ", name)) return;
    if (!promptForInput("Enter seller's username: ", sellerName)) return;
    name = trim(name);
    sellerName = trim(sellerName);

    // The game must exist in the available games and be sold by the specified seller
    const Game* game = games.findGame(name, sellerName);
    if (game == nullptr) {
        displayError("That game is not for sale.");
        return;
    }
    // Check if the game was listed in this session; if so, it cannot be bought until the next session
    if (game->wasListedThisSession()) {
        displayError("That game was listed in this session and cannot be bought until the next session.");
        return;
    }

    // The buyer must not already own the game, must have enough credit, and the seller must be able to receive the credit without exceeding the maximum account credit
    string buyerName = session.getCurrentUser().getUsername();
    if (games.ownsGame(buyerName, name)) {
        displayError("You already own that game.");
        return;
    }
    // Check if the buyer has enough credit
    User* buyer = users.findUser(buyerName);
    if (!buyer->hasCredit(game->getPrice())) {
        displayError("You do not have enough credit to buy that game.");
        return;
    }
    // Check if the seller can receive the credit without exceeding the maximum account credit
    User* seller = users.findUser(sellerName);
    if (seller != nullptr && !seller->canReceive(game->getPrice())) {
        displayError("The seller's account cannot hold that much credit.");
        return;
    }

    // Deduct the price from the buyer's credit, add it to the seller's credit (if the seller exists), add the game to the buyer's collection, write a buy record to the transaction file, and display a success message
    double price = game->getPrice();
    buyer->deductCredit(price);
    if (seller != nullptr) seller->addCredit(price);
    games.addToCollection(buyerName, name);
    files.writeBuyRecord(*game, buyerName);
    displayMessage("Bought " + name + " for $" + formatMoney(price, 0) + " from " + sellerName + ".");
}


// Handles the process of refunding credit from a seller to a buyer (admin only)
void FrontEnd::handleRefund() {
    string buyerName;
    string sellerName;
    string amountText;
    if (!promptForInput("Enter buyer's username: ", buyerName)) return;
    if (!promptForInput("Enter seller's username: ", sellerName)) return;
    if (!promptForInput("Enter amount of credit to refund: ", amountText)) return;
    buyerName = trim(buyerName);
    sellerName = trim(sellerName);

    // The buyer and seller must both exist in the user store
    User* buyer = users.findUser(buyerName);
    User* seller = users.findUser(sellerName);
    if (buyer == nullptr || seller == nullptr) {
        displayError("The buyer and the seller must both be current users.");
        return;
    }

    // The amount must be a valid number greater than 0.0, the seller must have enough credit to refund, and the buyer must be able to receive the credit without exceeding the maximum account credit
    double amount;
    if (!parseMoney(amountText, amount) || amount <= 0.0) {
        displayError("That is not a valid amount of credit.");
        return;
    }
    if (!seller->hasCredit(amount)) {
        displayError("The seller does not have enough credit for that refund.");
        return;
    }
    if (!buyer->canReceive(amount)) {
        displayError("That would exceed the maximum account credit.");
        return;
    }

    // Deduct the amount from the seller's credit, add it to the buyer's credit, write a refund record to the transaction file, and display a success message
    seller->deductCredit(amount);
    buyer->addCredit(amount);
    files.writeRefundRecord(buyerName, sellerName, amount);
    displayMessage("Refunded $" + formatMoney(amount, 0) + " from " + sellerName + " to " + buyerName + ".");
}


// Handles the process of adding credit to a user's account
void FrontEnd::handleAddCredit() {
    string amountText;
    string username = session.getCurrentUser().getUsername();
    if (!promptForInput("Enter amount of credit to add: ", amountText)) return;
    if (session.isAdmin() && !promptForInput("Enter username to credit: ", username)) return;
    username = trim(username);

    // The amount must be a valid number greater than 0.0
    double amount;
    if (!parseMoney(amountText, amount) || amount <= 0.0) {
        displayError("That is not a valid amount of credit.");
        return;
    }

    // The username must exist in the user store
    User* account = users.findUser(username);
    if (account == nullptr) {
        displayError("No such user.");
        return;
    }

    // Check that the total credit added to this account during this session does not exceed the maximum allowed per session, and that the account can receive the additional credit without exceeding the maximum account credit
    if (session.getAddedCredit(username) + amount > MAX_CREDIT_PER_SESSION + 0.001) {
        displayError("A maximum of $1000.00 can be added to an account per session.");
        return;
    }
    if (!account->canReceive(amount)) {
        displayError("That would exceed the maximum account credit.");
        return;
    }

    // Add the credit to the user's account, record the added credit for this session, write an add credit record to the transaction file, and display a success message
    account->addCredit(amount);
    session.recordAdded(username, amount);
    files.writeAddCreditRecord(*account, amount);
    displayMessage("Added $" + formatMoney(amount, 0) + " credit to " + username + ".");
}


// Helper function for user interaction
bool FrontEnd::promptForInput(const string& message, string& response) {
    output << message << flush;
    bool gotLine = static_cast<bool>(getline(input, response));
    if (gotLine && !response.empty() && response.back() == '\r') response.pop_back();
    return gotLine;
}

// Determines if the current user is permitted to perform the specified command based on their account type
bool FrontEnd::isPermitted(const string& command) const {
    const User& user = session.getCurrentUser();
    if (command == "create" || command == "delete") {
        return user.canAlterUsers();
    }
    if (command == "refund" || command == "listaccounts") {
        return user.isAdmin();
    }
    if (command == "sell") return user.canSell();
    if (command == "buy") return user.canBuy();
    return true;
}

// Prints normal output
void FrontEnd::displayMessage(const string& message) {
    output << message << endl;
}

// Prints error output
void FrontEnd::displayError(const string& message) {
    displayMessage("Error: " + message);
}
