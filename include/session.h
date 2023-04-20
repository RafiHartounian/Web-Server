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
  boost::asio::streambuf req_buf_;
  bool start();
  std::string handle_read(const boost::system::error_code &error, size_t bytes_transferred);
  bool handle_write(const boost::system::error_code &error);	

private:
  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];
};
