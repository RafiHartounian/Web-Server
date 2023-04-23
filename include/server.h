#pragma once

#include "session.h"
#include "session_interface.h"

using boost::asio::ip::tcp;
class server {
public:
  server(session_interface &new_s, boost::asio::io_service &io_service,
         short port);
  bool start_accept();
  bool handle_accept(session_interface *new_session,
                     const boost::system::error_code &error);

private:
  boost::asio::io_service &io_service_;
  session_interface &session_;
  tcp::acceptor acceptor_;
};
