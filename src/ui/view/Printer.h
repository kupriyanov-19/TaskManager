#pragma once

#include <string>

namespace ui {
class Printer {
public:
    virtual void PrintString(const std::string&);

    virtual ~Printer() = default;
};
}
