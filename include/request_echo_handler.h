#ifndef REQUEST_ECHO_HANDLER_H
#define REQUEST_ECHO_HANDLER_H

#include "request_handler_interface.h"
#include <cstddef>
#include <string>
#include <vector>
#include "request.h"

class request_echo_handler : public request_handler_interface
{
  public:
    request_echo_handler();
    request_echo_handler(http::server::request request, size_t bytes_transferred);
    void set_request(http::server::request request, size_t bytes_transferred);
    http::server::reply get_reply();

  private:
    std::string request_body;
    size_t size;
    http::server::reply reply;

};

#endif