/*
logger.h: Boost.log driven severity logger. Writes to log.log in local directory.
*/

#pragma once

#include <boost/log/support/date_time.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

//Severity levels
#define INFO BOOST_LOG_SEV(my_logger::get(), boost::log::trivial::info)
#define ERRORLOG BOOST_LOG_SEV(my_logger::get(), boost::log::trivial::error)

//Logger singleton
typedef boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level> logger_t;

BOOST_LOG_GLOBAL_LOGGER(my_logger, logger_t)