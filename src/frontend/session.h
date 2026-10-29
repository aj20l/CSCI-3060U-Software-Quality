#include "game_manager.h"
#include "payment_processor.h"
#include "user_manager.h"

class Session {
public:
    Session() : userManager(&gameManager) {};

    bool login();
    bool logout();
    bool isActive();
    bool createUser();
    bool deleteUser();
    bool sell();
    bool buy();
    bool refund();
    bool addCredit();
    bool listActiveAccounts();

private:
    GameManager gameManager;
    UserManager userManager;
    PaymentProcessor paymentProcessor;

    User* currentUser = nullptr;
    bool sessionActive = false;
    bool sessionSellLimited = false;
    float sessionAddedCreditLimit = 0.0F;
};

