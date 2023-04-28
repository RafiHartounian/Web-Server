//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "config_parser.h"
#include "server.h"
#include "session.h"
#include <cstdlib>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/core.hpp>
#include <boost/log/sources/logger.hpp>

using boost::asio::ip::tcp;

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

// Signal handler for keyboard interrupt
// Called when the server is being shut down
void signal_logger(int signal)
{
  BOOST_LOG_TRIVIAL(fatal) << "Received signal " << signal << ", server is shutting down\n";
  exit(signal);
}

// Initialize main for logging
void log_init()
{
  int rotate_size = 10 * 1024 * 1024; // 10 MB
  std::string log_format = "[%TimeStamp%][%ThreadID%][%Severity%]: %Message%";

  // Add file log parameters
  logging::add_file_log(
      keywords::file_name = "log/server_%Y%m%d%H%M.log",
      keywords::rotation_size = rotate_size,
      keywords::time_based_rotation = logging::sinks::file::rotation_at_time_point(0, 0, 0), // Rotate at midnight
      keywords::format = log_format,
      keywords::auto_flush = true);

  // Add console log parameters
  logging::add_console_log(
      std::cout,
      keywords::format = log_format,
      keywords::auto_flush = true);

  // Filter out message with severity level lower than info
  logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);
}

int main(int argc, char *argv[])
{
  // Initialize the logging
  signal(SIGINT, signal_logger);
  log_init();
  logging::add_common_attributes();

  try
  {
    if (argc != 2)
    {
      // no config file was given
      BOOST_LOG_TRIVIAL(fatal) << "Usage: async_tcp_echo_server <port>";
      return -1;
    }

    NginxConfig config;
    NginxConfigParser parser;

    BOOST_LOG_TRIVIAL(info) << "Starting config parsing";

    bool config_is_valid = parser.Parse(argv[1], &config);
    if (!config_is_valid)
    {
      BOOST_LOG_TRIVIAL(fatal) << "Invalid config file provided";
      return -1;
    }
    BOOST_LOG_TRIVIAL(info) << "Parsing Complete";


    int max_port = 65535;
    int port = config.get_listen_port();
    if(port < 0 || port > max_port)
    {
      BOOST_LOG_TRIVIAL(fatal) << "Invalid Port number of " << std::to_string(port) << " provided.";
      return -1;
    }

    BOOST_LOG_TRIVIAL(info) << "Listening on port "
                            << std::to_string(port);

    boost::asio::io_service io_service;

    session s(io_service);

    server serv(s, io_service, config.get_listen_port());
    serv.start_accept();

    BOOST_LOG_TRIVIAL(info) << "Now accepting connections";

    io_service.run();
  }
  catch (std::exception &e)
  {
    BOOST_LOG_TRIVIAL(error) << "Exception: " << e.what();
  }

  return 0;
}
