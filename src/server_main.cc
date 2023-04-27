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
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <cstdlib>
#include <iostream>
#include <boost/log/trivial.hpp>
using boost::asio::ip::tcp;

int main(int argc, char *argv[])
{
  try
  {
    if (argc != 2)
    {
      BOOST_LOG_TRIVIAL(fatal) << "Usage: async_tcp_echo_server <port>\n";
      return 1;
    }

    NginxConfig config;
    NginxConfigParser parser;
    bool config_is_valid = parser.Parse(argv[1], &config);
    if (!config_is_valid)
    {
      BOOST_LOG_TRIVIAL(fatal) << "Invalid config file provided\n";
      return 1;
    }

    BOOST_LOG_TRIVIAL(info) << "Listening on port "
                            << std::to_string(config.get_listen_port()) << "\n";

    boost::asio::io_service io_service;

    session s(io_service);

    server serv(s, io_service, config.get_listen_port());
    serv.start_accept();

    io_service.run();
  }
  catch (std::exception &e)
  {
    BOOST_LOG_TRIVIAL(error) << "Exception: " << e.what() << "\n";
  }

  return 0;
}
