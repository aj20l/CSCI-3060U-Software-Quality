#include "transaction_processor.h"


void TransactionProcessors::addTransaction(const Transaction& tx) {
    sessionTransactions.push_back(tx);
}

bool TransactionProcessors::logSessionTransactions(const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) return false;

    for (const auto& tx : sessionTransactions) {
        if (tx.code == "01" || tx.code == "02" || tx.code == "06") {
            // Format: XX UUUUUUUUUUUUUUU TT CCCCCCCCC
            outFile << tx.code << " "
                    << padLeft(tx.buyerOrUser, 15) << " "
                    << padLeft(tx.sellerOrType, 2) << " "
                    << formatCredit(tx.amount, 9) << "\n";
        } 
        else if (tx.code == "05") {
            // Format: XX UUUUUUUUUUUUUUU SSSSSSSSSSSSSSS CCCCCCCCC
            outFile << tx.code << " "
                    << padLeft(tx.buyerOrUser, 15) << " "
                    << padLeft(tx.sellerOrType, 15) << " "
                    << formatCredit(tx.amount, 9) << "\n";
        } 
        else if (tx.code == "03") {
            // Format: XX IIIIIIIIIIIIIIIIIII SSSSSSSSSSSSS CCCCCC
            outFile << tx.code << " "
                    << padLeft(tx.gameName, 19) << " "
                    << padLeft(tx.sellerOrType, 14) << " "
                    << formatCredit(tx.amount, 6) << "\n";
        } 
        else if (tx.code == "04") {
            // Format: XX IIIIIIIIIIIIIIIIIII SSSSSSSSSSSSSSS UUUUUUUUUUUUUU PPPPPP
            outFile << tx.code << " "
                    << padLeft(tx.gameName, 19) << " "
                    << padLeft(tx.sellerOrType, 15) << " "
                    << padLeft(tx.buyerOrUser, 14) << " "
                    << formatCredit(tx.amount, 6) << "\n";
        }
    }

    // Every sequence ends with an end-of-session (00) transaction code
    outFile << "00 " << padLeft("", 15) << " " << padLeft("", 2) << " " << formatCredit(0.0, 9) << "\n";
    
    outFile.close();
    sessionTransactions.clear(); // Flush session storage after writing
    return true;
}

// Formats and exports the state updates to the companion application data files
bool TransactionProcessors::updateMasterFiles(const std::string& accountsFile, const std::vector<User>& activeUsers,
                        const std::string& gamesFile, const std::vector<Game>& availableGames) {
    
    // 1. Write Current User Accounts File (Fixed length: 28 characters + newline)
    std::ofstream userOut(accountsFile);
    if (!userOut.is_open()) return false;

    for (const auto& user : activeUsers) {
        userOut << padLeft(user.username, 15) << " "
                << padLeft(user.type, 2) << " "
                << formatCredit(user.credit, 9) << "\n";
    }
    // Terminal "END" line assignment
    userOut << padLeft("END", 15) << " " << padLeft("", 2) << " " << padLeft("", 9) << "\n";
    userOut.close();

    // 2. Write Available Games File (Fixed length: 42 characters + newline)
    std::ofstream gameOut(gamesFile);
    if (!gameOut.is_open()) return false;

    for (const auto& game : availableGames) {
        gameOut << padLeft(game.name, 19) << " " 
                << padLeft(game.sellerOrOwner, 15) << " " 
                << formatCredit(game.price, 6) << "\n";
    }
    // Terminal "END" line assignment
    gameOut << padLeft("END", 19) << " " << padLeft("", 15) << " " << padLeft("", 6) << "\n";
    gameOut.close();

    return true;
}

std::string TransactionProcessors::padLeft(const std::string& str, size_t length) {
    if (str.length() >= length) return str.substr(0, length);
    return str + std::string(length - str.length(), ' ');
}

std::string TransactionProcessors::formatCredit(double amount, size_t totalLength) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << amount;
    std::string str = ss.str();
    
    if (str.length() < totalLength) {
        return std::string(totalLength - str.length(), '0') + str;
    }
    return str;
}