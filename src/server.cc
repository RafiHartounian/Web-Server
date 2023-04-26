#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <cstdlib>
#include <iostream>

#include "server.h"
#include "session.h"

using boost::asio::ip::tcp;

server::server(session_interface &new_s, boost::asio::io_service &io_service,
               short port)
    : io_service_(io_service), session_(new_s),
      acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {}

bool server::start_accept()
{
  session_interface *new_session = session_.get_session(io_service_);
  if (new_session == NULL)
  {
    return false;
  }
  acceptor_.async_accept(new_session->socket(),
                         boost::bind(&server::handle_accept, this, new_session,
                                     boost::asio::placeholders::error));
  return true;
}

bool server::handle_accept(session_interface *new_session,
                           const boost::system::error_code &error)
{
  bool result = false;
  if (!error)
  {
    new_session->start();
    result = true;
  }
  else
  {
    delete new_session;
  }

  start_accept();
  return result;
}
