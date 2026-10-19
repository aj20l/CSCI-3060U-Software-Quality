#include <iostream>
#include <string>
#include "frontend.h"

void FrontEnd::startSession() {
    Interface();
}

void FrontEnd::Interface() {
    int choice = -1;

    while (choice != 0) {
        std::cout << "\n===== STORE OPTIONS =====\n";
        std::cout << "1. Login\n";
        std::cout << "2. Create Account\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter choice: ";

        if (!(std::cin >> choice)) {
            return;
        }

        switch (choice) {
        case 1:
            currentSession.login();
            break;
        case 2:
            std::cout << "Account creation will be added later\n";
            break;
        case 0:
            endSession();
            break;
        default:
            std::cout << "Invalid choice. Try again.\n";
        }
    }
}
