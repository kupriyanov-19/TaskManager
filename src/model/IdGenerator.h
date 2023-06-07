#pragma once

#include <atomic>
#include "Task.pb.h"

namespace model {
class IdGenerator {
public:
    explicit IdGenerator(int max_id = 0) : max_id_(max_id) {}

    virtual TaskId GenerateId();

    virtual ~IdGenerator() = default;
private:
    std::atomic<int> max_id_;
};
}