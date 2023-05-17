#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <cstdlib>
#include <iostream>

#include "server.h"
#include "handler_factories/static_handler_factory.h"
#include "handler_factories/echo_handler_factory.h"
#include "handler_factories/handler404factory.h"



using boost::asio::ip::tcp;

server::server(session_interface& new_s, boost::asio::io_service& io_service,
               short port)
  : io_service_(io_service), session_(new_s),
  acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {}

bool server::set_configured_paths(std::vector<path> paths)
{
  paths_ = paths;
  return true;
}

bool server::set_routes(std::map<std::string, request_handler_factory*> routes) {
  routes_ = routes;
  return true;
}

bool server::start_accept()
{
  session_interface* new_session = session_.get_session(io_service_);
  if (new_session == NULL)
  {
    return false;
  }
  for (path p : paths_) {
    switch (p.type) {
      // case endpoint_type::static_:
      //   routes_.emplace(p.endpoint, new static_handler_factory(p.endpoint, p));
      default:
        break;
    }

  }

  new_session->set_configured_paths(paths_);
  acceptor_.async_accept(new_session->socket(),
                         boost::bind(&server::handle_accept, this, new_session,
                           boost::asio::placeholders::error));
  return true;
}

bool server::handle_accept(session_interface* new_session,
                           const boost::system::error_code& error)
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
