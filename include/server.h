#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "session_interface.h"

using boost::asio::ip::tcp;
class server
{
public:
  server(session_interface& new_s, boost::asio::io_service& io_service,
         short port);
  bool start_accept();
  bool handle_accept(session_interface* new_session,
                     const boost::system::error_code& error);
  bool set_configured_paths(std::vector<path> paths);

private:
  boost::asio::io_service& io_service_;
  session_interface& session_;
  tcp::acceptor acceptor_;
  std::vector<path> paths_;
};
