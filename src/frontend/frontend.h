#include "session.h"

class FrontEnd {
public:
    FrontEnd() = default;
    void startSession();
    void loginInterface();
    void storeInterface();
    
private:
    Session currentSession;
};