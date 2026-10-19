#include "game_manager.h"
#include "payment_processor.h"
#include "user_manager.h"

class Session {
public:
    Session() = default;

    bool login();
    bool logout();
    bool isActive();
    bool createUser();
    bool deleteUser();
    bool sell();
    bool buy();
    bool refund();
    bool addCredit();

private:
    UserManager userManager;
    GameManager gameManager;
    PaymentProcessor paymentProcessor;

    User* currentUser = nullptr;
    bool sessionActive = false;
    bool sessionSellLimited = false;
    float sessionAddedCreditLimit = 0.0F;
};

