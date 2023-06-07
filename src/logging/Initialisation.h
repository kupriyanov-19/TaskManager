#pragma once

#include <string>
#include <boost/log/trivial.hpp>

class ConsoleLogging {
public:
    explicit ConsoleLogging(const boost::log::trivial::severity_level& severity);
};

class FileLogging {
public:
    explicit FileLogging(const std::string& filename, const boost::log::trivial::severity_level& severity);
};