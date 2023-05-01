#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/log/trivial.hpp>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <regex>

#include "request_echo_handler.h"
#include "request_error_handler.h"
#include "request_parser.h"
#include "request_static_handler.h"
#include "session.h"

using boost::asio::ip::tcp;

session::session(boost::asio::io_service& io_service): socket_(io_service) {}

tcp::socket& session::socket() { return socket_; }

bool session::set_configured_paths(std::vector<path> paths) {
  paths_ = paths;
  return true;
}

bool session::start() {
  boost::system::error_code error_code;
  auto remote_ep = socket_.remote_endpoint(error_code);
  dest_ip = error_code ? boost::asio::ip::address::from_string("127.0.0.1")
    : remote_ep.address(); // local host address
  boost::asio::async_read_until(
      socket_, buf, "\r\n\r\n",
      boost::bind(&session::handle_read, this, boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
  return true;
}

std::string session::handle_read(const boost::system::error_code& error,
                                 size_t bytes_transferred) {
  std::string req = "";
  http::server::request_parser::parse_result bad_res = http::server::request_parser::parse_result::bad;
  http::server::request_parser::parse_result good_res = http::server::request_parser::parse_result::good;
  http::server::request_parser::parse_result indeterminate_res = http::server::request_parser::parse_result::indeterminate;
  if (!error) {
    http::server::request_parser::parse_result res;
    // keep an eye on this, maybe need to be in the while statement
    std::istreambuf_iterator<char> st{&buf}, end;
    boost::tie(res, std::ignore) = req_parser.parse(request_, st, end);
    while (res == indeterminate_res) {
      std::istreambuf_iterator<char> st{&buf}, end;
      boost::tie(res, std::ignore) = req_parser.parse(request_, st, end);
    }

    request_handler_interface* req_handler_int;
    if (res == good_res) {
      // LOG info about good request
      path req_ep = get_endpoint();
      if (req_ep.type == endpoint_type::echo) {
        req_handler_int = new request_echo_handler(request_, bytes_transferred);
      }
      else if (req_ep.type == endpoint_type::static_) {
        req_handler_int = new request_static_handler(request_, req_ep.root);
      }
      else {
        req_handler_int = new request_error_handler(
            http::server::reply::status_type::not_found);
      }
    }
    else if (res == bad_res) {
   // lOG info about bad request
      req_handler_int = new request_error_handler(
          http::server::reply::status_type::bad_request);
    }
    else if (res == indeterminate_res) {
    }
    else {
   // DO SOME ERROR HERE IF POSSIBLE or default to 404
    }
    write_to_socket(req_handler_int);
  }
  else {
    log_error("handle_read", "request parser invalid");
    delete this;
  }

  return request_.original_req;
}

path session::get_endpoint() {
  path result{ endpoint_type::invalid, "" };
  std::regex path_regex(R"((/[a-zA-Z_0-9]+)*/[a-zA-Z_0-9.]+$)");
  std::smatch match;

  if (std::regex_search(request_.uri, match, path_regex)) {
    std::string partial_path = match.str(1);
    for (const auto& p : paths_) {
      if (partial_path == p.endpoint) {
        result = p;
        break;
      }
      if (request_.uri == p.endpoint) {
        return p;
      }
    }
  }
  return result;
}
void session::write_to_socket(request_handler_interface* req_h) {
  boost::system::error_code ec;
  socket_.remote_endpoint(ec);
  if (!ec) {
    boost::asio::write(socket_, req_h->get_reply().to_buffers());
    handle_write(boost::system::error_code());
  }
  else {
    // log error
    return;
  }
}

bool session::handle_write(const boost::system::error_code& error) {
  if (!error) {
    boost::system::error_code ignored_err;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_err);
    log_info("handle_write", "connection closed successfully");
    return true;
  }
  else {
    log_error("handle_write", "error closing the connection");
    delete this;
    return false;
  }
}

session_interface* session::get_session(boost::asio::io_service& io_service) {
  return new session(io_service);
}

void session::log_info(std::string func_name, std::string message) {
  BOOST_LOG_TRIVIAL(info) << "Client IP: " << dest_ip.to_string()
    << "\tsession::" << func_name << ":" << message;
}

void session::log_error(std::string func_name, std::string message) {
  BOOST_LOG_TRIVIAL(error) << "Client IP: " << dest_ip.to_string()
    << "\tsession::" << func_name << ":" << message;
}