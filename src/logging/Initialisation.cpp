#include "Initialisation.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/filesystem.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>

namespace lg = boost::log;
typedef lg::sinks::synchronous_sink<lg::sinks::text_ostream_backend> text_sink;

void Formatter(lg::record_view const& rec, lg::formatting_ostream& stream) {
    stream << lg::extract<unsigned int>("LineID", rec) << " ";
    stream << "[" << lg::extract<boost::posix_time::ptime>("TimeStamp", rec) << "] ";
    stream << "<" << rec[lg::trivial::severity] << "> ";
    lg::value_ref<std::string> full_path = lg::extract<std::string>("File", rec);
    stream << "(" << boost::filesystem::path(full_path.get()).filename().string() << ":";
    stream << lg::extract<int>("Line", rec) << ") ";
    stream << "|" << lg::extract<std::string>("Function", rec) << "| ";
    stream << rec[lg::expressions::smessage];
}

void Initialisation() {
    lg::add_common_attributes();
    lg::core::get()->add_global_attribute("Line", lg::attributes::mutable_constant<int>(0));
    lg::core::get()->add_global_attribute("File", lg::attributes::mutable_constant<std::string>(""));
    lg::core::get()->add_global_attribute("Function", lg::attributes::mutable_constant<std::string>(""));
}

ConsoleLogging::ConsoleLogging(const lg::trivial::severity_level& severity) {
    Initialisation();

    boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();
    sink->locked_backend()->auto_flush(true);
    sink->locked_backend()->add_stream(boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));

    sink->set_formatter(&Formatter);
    sink->set_filter(lg::trivial::severity >= severity);

    lg::core::get()->add_sink(sink);
}

FileLogging::FileLogging(const std::string& filename, const lg::trivial::severity_level& severity) {
    Initialisation();

    boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();
    sink->locked_backend()->auto_flush(true);
  //  sink->locked_backend()->add_stream(boost::make_shared<std::ofstream>(filename));

    sink->set_formatter(&Formatter);
    sink->set_filter(lg::trivial::severity >= severity);

    lg::core::get()->add_sink(sink);
}