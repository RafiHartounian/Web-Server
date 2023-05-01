#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/log/trivial.hpp>
#include <boost/lexical_cast.hpp>
#include <cstdlib>
#include <iostream>

#include "reply.h"
#include "session.h"

using boost::asio::ip::tcp;

session::session(boost::asio::io_service& io_service): socket_(io_service) {}

tcp::socket& session::socket() { return socket_; }

bool session::set_configured_paths(std::vector<path> paths)
{
  paths_ = paths;
  return true;
}

bool session::start()
{
  boost::system::error_code ec;
  socket_.remote_endpoint(ec);
  if (ec)
  {
    client_ip_ = boost::asio::ip::address::from_string("127.0.0.1");;
  }
  else
  {
    client_ip_ = socket_.remote_endpoint(ec).address();
  }
  log_info("start", "Accepting incoming requests.");
  boost::asio::async_read_until(
      socket_, req_buf_, "\r\n\r\n",
      boost::bind(&session::handle_read, this, boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
  return true;
}

std::string session::handle_read(const boost::system::error_code& error,
                                 size_t bytes_transferred)
{
  std::string req = "";
  if (!error)
  {
    log_info("handle_read", "request parser valid");
    std::istream req_stream(&req_buf_);
    std::string req_build;
    while (req_build.find("\r\n\r\n") == std::string::npos)
    {
      char req_tok;
      req_stream.get(req_tok);
      req_build += req_tok;
    }
    req = req_build;
    log_info("handle_read", "request parser contents: " + req);

    http::server::reply res;
    res.status = http::server::reply::ok;
    res.content = req_build;
    res.headers.resize(2);
    res.headers[0].name = "Content-Length";
    res.headers[0].value = boost::lexical_cast<std::string>(res.content.size());
    res.headers[1].name = "Content-Type";
    res.headers[1].value = "text/plain";
    boost::asio::async_write(socket_,
                             //  res.to_buffers(req_build, bytes_transferred),
                             res.to_buffers(),
                             boost::bind(&session::handle_write, this,
                               boost::asio::placeholders::error));
    req_build.clear();
  }
  else
  {
    log_error("handle_read", "request parser invalid");
    delete this;
  }

  return req;
}

bool session::handle_write(const boost::system::error_code& error)
{
  if (!error)
  {
    /**boost::asio::async_read_until(
  socket_, req_buf_, "\r\n\r\n",
  boost::bind(&session::handle_read, this,
  boost::asio::placeholders::error,
  boost::asio::placeholders::bytes_transferred));**/
    boost::system::error_code ignored_err;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_err);
    log_info("handle_write", "connection closed successfully");
    return true;
  }
  else
  {
    log_error("handle_write", "error closing the connection");
    delete this;
    return false;
  }
}

session_interface* session::get_session(boost::asio::io_service& io_service)
{
  return new session(io_service);
}

void session::log_info(std::string func_name, std::string message)
{
  BOOST_LOG_TRIVIAL(info) << "Client IP: " << client_ip_.to_string() << "\tsession::" << func_name << ":" << message;
}

void session::log_error(std::string func_name, std::string message)
{
  BOOST_LOG_TRIVIAL(error) << "Client IP: " << client_ip_.to_string() << "\tsession::" << func_name << ":" << message;
}