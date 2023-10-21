#include "radar_global_logger.h"

#include <boost/core/null_deleter.hpp>
#include <boost/filesystem.hpp>
#include <boost/log/core/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <chrono>
#include <fstream>
#include <ostream>
#include <string>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;

BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity",
                            logging::trivial::severity_level)

BOOST_LOG_GLOBAL_LOGGER_INIT(logger, src::severity_logger_mt) {
  src::severity_logger_mt<logging::trivial::severity_level> logger;

  // add attributes
  logger.add_attribute("LineID", attrs::counter<unsigned int>(
                                     1)); // lines are sequentially numbered
  logger.add_attribute("TimeStamp",
                       attrs::local_clock()); // each log line gets a timestamp

  // add a text sink
  typedef sinks::synchronous_sink<sinks::text_ostream_backend> text_sink;
  boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();

  // add a logfile stream to our sink
  auto now = std::chrono::system_clock::now();
  std::time_t now_c = std::chrono::system_clock::to_time_t(now);
  struct tm *parts = std::localtime(&now_c);

  boost::filesystem::path dir("logs");

  if (!(boost::filesystem::exists(dir))) {
    boost::filesystem::create_directory(dir);
  }

  auto logFileName = "logs/log_" + std::to_string(1900 + parts->tm_year) + "_" +
                     std::to_string(1 + parts->tm_mon) + "_" +
                     std::to_string(parts->tm_mday) + ".log";
  sink->locked_backend()->add_stream(boost::make_shared<std::ofstream>(
      logFileName, std::ios::out | std::ios::app));

  //    // add "console" output stream to our sink
  //    boost::shared_ptr<std::ostream> consoleStream(&std::clog,
  //    boost::null_deleter());
  //    sink->locked_backend()->add_stream(consoleStream);

  // specify the format of the log message
  logging::formatter formatter =
      expr::stream << std::setw(7) << std::setfill('0') << line_id
                   << std::setfill(' ') << " | "
                   << expr::format_date_time(timestamp, "%Y-%m-%d, %H:%M:%S.%f")
                   << " "
                   << "[" << logging::trivial::severity << "]"
                   << " - " << expr::smessage;

  sink->set_formatter(formatter);

  // just log messages with severity >= SEVERITY_THRESHOLD are written
  //    sink->set_filter(severity >= SEVERITY_THRESHOLD);

  // "register" our sink
  logging::core::get()->add_sink(sink);

  return logger;
}
