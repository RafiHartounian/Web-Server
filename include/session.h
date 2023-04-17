#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <cstdlib>
#include <iostream>

using boost::asio::ip::tcp;

class session {
public:
  session(boost::asio::io_service &io_service);
  tcp::socket &socket();
  void start();

private:
  void handle_read(const boost::system::error_code &error,
                   size_t bytes_transferred);
  void handle_write(const boost::system::error_code &error);
  boost::asio::streambuf req_buf_;
  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];
};
