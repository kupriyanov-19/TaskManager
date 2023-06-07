#include "Printer.h"
#include <iostream>

namespace ui {
void Printer::PrintString(const std::string& string) {
    std::cout << string;
}
}