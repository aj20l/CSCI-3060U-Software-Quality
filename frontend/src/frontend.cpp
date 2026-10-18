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
    cout << "\n===== STORE OPTIONS =====\n";
    cout << "1. Login\n";
    cout << "2. Create Account\n";

    cout << "Enter choice: ";

    cin >> choice;

    switch (choice) {
        case 1:
            if(!currentSession.login()) {
                return;
            }
            break;

        case 2:
            cout << "Account Creation will be added later\n";
            break;

        default:
            cout << "Invalid choice. Try again.\n";
    }

} while (choice != 0);
	
	
	
}

