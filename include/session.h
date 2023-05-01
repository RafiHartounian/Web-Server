#pragma once

#include "session_interface.h"
#include "request_handler_interface.h"
#include "request_parser.h"
#include "request.h"

#include <boost/asio/ip/address.hpp>
#include <boost/bind.hpp>
#include <cstdlib>
#include <iostream>

using boost::asio::ip::tcp;

class session: public session_interface
{
public:
  session(boost::asio::io_service& io_service);
  tcp::socket& socket();
  boost::asio::streambuf req_buf_;
  bool start();
  std::string handle_read(const boost::system::error_code& error,
                          size_t bytes_transferred);
  bool handle_write(const boost::system::error_code& error);
  session_interface* get_session(boost::asio::io_service& io_service);
  void write_to_socket(request_handler_interface* handler);
  bool set_configured_paths(std::vector<path> paths);
  path get_endpoint();

private:
  void log_info(std::string func_name, std::string message);
  void log_error(std::string func_name, std::string message);

  tcp::socket socket_;
  enum
  {
    max_length = 1024
  };
  char data_[max_length];
  boost::asio::ip::address client_ip_;
  http::server::request_parser request_parser_;
  http::server::request request_;
  std::vector<path> paths_;
};
