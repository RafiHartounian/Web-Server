#include "request_echo_handler.h"
#include <cstddef>
#include <string>
#include <vector>


request_echo_handler::request_echo_handler()
{
  request_body = "";
  size = 0;
}

request_echo_handler::request_echo_handler(http::server::request request, size_t byte_transferred) 
  : request_body(request.original_req), size(byte_transferred)
{

}

// Set specific message
void request_echo_handler::set_request(http::server::request request, size_t byte_transferred)
{
  request_body = request.original_req;
  size = byte_transferred;
}

// Construct echo reply
http::server::reply request_echo_handler::get_reply()
{
  reply.status = http::server::reply::ok;
  reply.content = request_body;
  reply.headers.resize(2);
  reply.headers[0].name = "Content-Length";
  reply.headers[0].value = std::to_string(reply.content.size());
  reply.headers[1].name = "Content-Type";
  reply.headers[1].value = "text/plain";
  return reply;
}