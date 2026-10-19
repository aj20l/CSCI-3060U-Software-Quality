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

        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout<<"---------------------------\n";
            if(!currentSession.login()) {

                return;
            }
            break;
        case 2:
            std::cout<<"---------------------------\n";
            std::cout << "Account Creation will be added later\n";
            break;
        case 3:
            std::cout<<"---------------------------\n";
            std::cout << "Games viewing will be added later\n";
            break;
        default:
            std::cout<<"---------------------------\n";
            std::cout << "Invalid choice. Try again.\n";
        }
    }
}
