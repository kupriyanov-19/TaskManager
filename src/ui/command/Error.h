#pragma once

namespace ui::command {
enum class Error {
    INCORRECT_PARENT_ID,
    NO_TASK_WITH_SUCH_ID,
    CANNOT_SAVE_TO_FILE,
    CANNOT_LOAD_FROM_FILE
};
}