#pragma once

#include <boost/log/trivial.hpp>
#include <boost/log/attributes/mutable_constant.hpp>

#define LOG(severity, message)           \
   LOG_LOCATION                          \
   BOOST_LOG_TRIVIAL(severity) << message;

#define LOG_LOCATION                                                                    \
   boost::log::attribute_cast<boost::log::attributes::mutable_constant<int>>(           \
          boost::log::core::get()->get_global_attributes()["Line"]).set(__LINE__);      \
   boost::log::attribute_cast<boost::log::attributes::mutable_constant<std::string>>(   \
          boost::log::core::get()->get_global_attributes()["File"]).set(__FILE__);      \
   boost::log::attribute_cast<boost::log::attributes::mutable_constant<std::string>>(   \
          boost::log::core::get()->get_global_attributes()["Function"]).set(__func__);
