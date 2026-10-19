#include "session.h"

class FrontEnd {
public:
    FrontEnd() = default;
    void Interface();
    void startSession();

private:
    Session currentSession;
};