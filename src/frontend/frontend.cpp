#include <iostream>
#include "session.cpp"





class FrontEnd {
    public:
        FrontEnd() = default;
		void Interface();
        void startSession() {
        	Interface();
//            if(!currentSession.login()) {
//                return;
//            }
            while(currentSession.isActive()) {
                std::string transaction;
                std::cout << "Enter transaction: ";
                std::cin >> transaction;
            }
        }
        void endSession() {

        }

    private:
        Session currentSession;  

};

void FrontEnd::Interface(){
	int choice;

do {

    std::cout << "\n===== STORE OPTIONS =====\n";
    std::cout << "1. Login\n";
    std::cout << "2. Create Account\n";
    std::cout << "3. View Games\n";
    
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

} while (choice != 0);
	
	
	
}

