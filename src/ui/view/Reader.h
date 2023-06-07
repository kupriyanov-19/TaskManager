#pragma once

#include <string>

namespace ui {
class Reader {
public:
    virtual std::string ReadString();

    virtual ~Reader() = default;
};
}