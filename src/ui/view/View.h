#pragma once

#include <string>
#include <ctime>
#include <memory>
#include <vector>
#include <optional>
#include "Reader.h"
#include "Printer.h"
#include "utilities/Convert.h"
#include "Task.pb.h"
#include "TaskStructures.pb.h"
#include "ui/step/Type.h"
#include "ui/command/Error.h"

namespace ui {
class View {
public:
    View(const std::shared_ptr<Reader>&, const std::shared_ptr<Printer>&);
    virtual ~View() = default;
public:
    virtual void PrintGlobalHelp();
    virtual void PrintHelp();
    virtual void PrintQuit();
    virtual step::Type ReadFirstCommand();
    virtual step::Type ReadCommand();
    virtual TaskId ReadId(const std::string& wizard);
    virtual TaskId ReadParentId(const std::string& wizard);
    virtual std::string ReadTitle(const std::string& wizard);
    virtual Task::Priority ReadPriority(const std::string& wizard);
    virtual google::protobuf::Timestamp ReadDate(const std::string& wizard);
    virtual std::vector<std::string> ReadLabels(const std::string& wizard);
    virtual void PrintString(const std::string& str) {printer_->PrintString(str+"\n");}
    virtual bool Confirm();
    virtual bool ReadIfPrintSubtasks(const std::string& wizard);
    virtual std::string ReadLabel(const std::string& wizard);
    virtual TasksSortBy ReadSortBy(const std::string& wizard);

    virtual std::string ReadName(const std::string& wizard);
    virtual std::string ReadPassword(const std::string& wizard);
    virtual std::string ReadIP();
    virtual bool ReadCreateData(std::string&, std::string&);
    virtual void PrintCreateResult(bool);
    virtual bool ReadEnterData(std::string&, std::string&);
    virtual void PrintEnterResult(bool, std::string&);
    virtual bool ReadDeleteData(std::string&, std::string&);
    virtual void PrintDeleteResult(bool, std::string&);
    virtual void PrintCheckResult(bool);

    virtual void PrintManyTasksWithId(const ManyTasksWithId&);
    virtual void PrintCompositeTask(const CompositeTask&);
    virtual void PrintManyCompositeTasks(const ManyCompositeTasks&);
    virtual void PrintError(const command::Error&);
private:
    const std::shared_ptr<Reader> reader_;
    const std::shared_ptr<Printer> printer_;
};
}
