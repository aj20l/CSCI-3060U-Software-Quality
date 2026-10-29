#pragma once
#include <string>

enum Privilege {
    ADMIN,
    FULLSTANDARD,
    BUYSTANDARD,
    SELLSTANDARD,
    NONUSER
};

inline Privilege convert(const std::string& str) {
    if(str == "ADMIN") return ADMIN;
    else if(str == "FULLSTANDARD") return FULLSTANDARD;
    else if(str == "BUYSTANDARD") return BUYSTANDARD;
    else if(str == "SELLSTANDARD") return SELLSTANDARD;
    else return NONUSER;
};