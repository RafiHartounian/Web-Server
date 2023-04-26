#pragma once

#include "session_interface.h"
#include <boost/bind.hpp>
#include <cstdlib>
#include <iostream>

using boost::asio::ip::tcp;

class session : public session_interface
{
public:
  session(boost::asio::io_service &io_service);
  tcp::socket &socket();
  boost::asio::streambuf req_buf_;
  bool start();
  std::string handle_read(const boost::system::error_code &error,
                          size_t bytes_transferred);
  bool handle_write(const boost::system::error_code &error);
  session_interface *get_session(boost::asio::io_service &io_service);

private:
  tcp::socket socket_;
  enum
  {
    max_length = 1024
  };
  char data_[max_length];
};
