#include "Reader.h"
#include <iostream>

namespace ui {
std::string Reader::ReadString() {
    std::string result;
    getline(std::cin, result);
    return result;
}
}