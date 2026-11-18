// Games Distribution Front End


// INPUT: current_accounts.txt, available_games.txt
// OUTPUT: daily_transaction.txt
// RUN: ./FrontEnd or to run with a sample test file(must be ran through cmd): ./FrontEnd current_accounts.txt available_games.txt daily_transaction.txt < test.txt
#include <iostream>
#include <string>
#include "FileManager.h"
#include "FrontEnd.h"
using namespace std;

int main(int argc, char* argv[]) {
    // Determine the file names for accounts, games, and transactions based on command line arguments or use default values 
    string accountsFile = (argc > 1) ? argv[1] : "../data/currentaccounts.txt";
    string gamesFile = (argc > 2) ? argv[2] : "../data/availablegames.txt";
    string collectionFile = (argc > 3) ? argv[3] : "../data/gamescollection.txt";
    string transactionFile = (argc > 4) ? argv[4] : "../data/transout.atf";

    FileManager files(accountsFile, gamesFile, collectionFile, transactionFile);
    FrontEnd frontEnd(cin, cout, files);
    frontEnd.run();
    return 0;
}