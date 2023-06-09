#include "TaskPersister.h"
#include "utilities/CreateProtoObjects.h"
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/util/delimited_message_util.h>

bool TaskPersister::Save(const ManyHierarchicalTasks& tasks) {
    std::unique_lock lock(mutex_);
    std::ofstream file(filename_);
    if (!file.is_open()) return false;

    Task t(CreateTask(password_));
    google::protobuf::util::SerializeDelimitedToOstream(t, &file);

    for (const auto &[id, task]: tasks) {
        google::protobuf::util::SerializeDelimitedToOstream(id, &file);
        google::protobuf::util::SerializeDelimitedToOstream(task, &file);
    }
    file.close();
    return true;
}

std::optional<ManyHierarchicalTasks> TaskPersister::Load() {
    std::shared_lock lock(mutex_);
    ManyHierarchicalTasks result;

    std::ifstream file(filename_);
    if (!file.is_open() || file.fail()) return std::nullopt;

    std::unique_ptr<google::protobuf::io::ZeroCopyInputStream> input =
            std::make_unique<google::protobuf::io::IstreamInputStream>(&file);
    HierarchicalTask task;
    TaskId id; Task t;

    google::protobuf::util::ParseDelimitedFromZeroCopyStream(&t, input.get(), nullptr);
    if (t.title()!=password_) return std::nullopt;

    while (google::protobuf::util::ParseDelimitedFromZeroCopyStream(&id, input.get(), nullptr) &&
           google::protobuf::util::ParseDelimitedFromZeroCopyStream(&task, input.get(), nullptr)) {
        result.push_back({id, task});
        task.clear_parent();
        task.clear_task();
        id.clear_value();
    }
    file.close();
    return result;
}