#include "logger.h"

namespace attrs = boost::log::attributes;
namespace expr = boost::log::expressions;
namespace logging = boost::log;

BOOST_LOG_GLOBAL_LOGGER_INIT(my_logger, logger_t) {
	logger_t lg;

	logging::add_common_attributes();

	logging::add_file_log(
		boost::log::keywords::file_name = "log.log",
		boost::log::keywords::format = (
			expr::stream << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%H:%M:%S")
			<< " [" << expr::attr<boost::log::trivial::severity_level>("Severity") << "]"
			<< expr::smessage
			)
	);

	logging::core::get()->set_filter(
		logging::trivial::severity >= logging::trivial::debug
	);

	return lg;
}