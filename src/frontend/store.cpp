#include <iostream>
#include "frontend.h"

int main(int argc, char* argv[]) {
    for(int i = 0; i < argc; ++i) {
        if(std::string(argv[i]) == "--test") {
            Mode::testMode = true;
        }
    }

    FrontEnd front;
    front.startSession();
    return 0;
}