#include "request_error_handler.h"

request_error_handler::request_error_handler()
{
  err_code = http::server::reply::status_type::ok;
}

request_error_handler::request_error_handler(http::server::reply::status_type ec) : err_code(ec)
{

}

void request_error_handler::set_error_code(http::server::reply::status_type ec)
{
  err_code = ec;
}

http::server::reply request_error_handler::get_reply()
{
  return reply.stock_reply(err_code);
}