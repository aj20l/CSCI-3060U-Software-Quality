#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "user.h"
#include "game.h"

struct Transaction {
    std::string code; // 00, 01, 02, 03, 04, 05, 06
    std::string buyerOrUser;
    std::string sellerOrType;
    std::string gameName;
    double amount; // Used for credit, refunds, or prices
};

class TransactionProcessors {
    public:
        void addTransaction(const Transaction& tx);
        // Writes out the Daily Transaction File at the end of the session
        bool logSessionTransactions(const std::string& filename);
        // Formats and exports the state updates to the companion application data files
        bool updateMasterFiles(const std::string& accountsFile, const std::vector<User>& activeUsers,
                            const std::string& gamesFile, const std::vector<Game>& availableGames);

    private:
        std::vector<Transaction> sessionTransactions;

        std::string padLeft(const std::string& str, size_t length);
        std::string formatCredit(double amount, size_t totalLength);

};
