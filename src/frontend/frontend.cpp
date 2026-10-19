#include <iostream>
#include <string>
#include "frontend.h"

void FrontEnd::startSession() {
    loginInterface();
}

void FrontEnd::loginInterface() {
    int choice = -1;

    while (choice != 0) {
        std::cout << "\n===== STORE OPTIONS =====\n";
        std::cout << "1. Login\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter choice: ";

        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout<<"---------------------------\n";
            if(currentSession.login()) {
                storeInterface();
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
        case 0:
            std::cout<<"---------------------------\n";
            std::cout << "Exiting\n";
            break;
        default:
            std::cout<<"---------------------------\n";
            std::cout << "Invalid choice. Try again.\n";
        }
    }
}

void FrontEnd::storeInterface() {
    int choice = -1;

    while (choice != 0) {
        std::cout << "\n===== STORE OPTIONS =====\n";
        std::cout << "1. Logout\n";
        std::cout << "2. Create\n";
        std::cout << "3. Delete\n";
        std::cout << "4. Sell\n";
        std::cout << "5. Buy\n";
        std::cout << "6. Refund\n";
        std::cout << "7. Add Credit\n";
        std::cout << "Enter choice: ";

        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout<<"---------------------------\n";
            if(currentSession.logout()) {

                return;
            }
            break;
        case 2:
            std::cout<<"---------------------------\n";
            std::cout << "";
            if(currentSession.createUser()) {

                return;
            }
            break;
        case 3:
            std::cout<<"---------------------------\n";
            std::cout << "";
            if(currentSession.deleteUser()) {

                return;
            }
            break;
        case 4:
            std::cout<<"---------------------------\n";
            std::cout << "";
            if(currentSession.sell()) {

                return;
            }
            break;
        case 5:
            std::cout<<"---------------------------\n";
            std::cout << "";
            if(currentSession.buy()) {

                return;
            }
            break;
        case 6:
            std::cout<<"---------------------------\n";
            std::cout << "";
            if(currentSession.refund()) {

                return;
            }
            break;
        case 7:
            std::cout<<"---------------------------\n";
            std::cout << "";
            if(currentSession.addCredit()) {

                return;
            }
            break;
        default:
            std::cout<<"---------------------------\n";
            std::cout << "Invalid choice. Try again.\n";
        }
    }
}