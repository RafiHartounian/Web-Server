#ifndef REQUEST_ERROR_HANDLER_H
#define REQUEST_ERROR_HANDLER_H

#include "request_handler_interface.h"
#include "reply.h"

class request_error_handler : public request_handler_interface
{
  public:
    request_error_handler();
    request_error_handler(http::server::reply::status_type error_code);
    void set_error_code(http::server::reply::status_type error_code);
    http::server::reply get_reply();

  private:
    http::server::reply::status_type err_code;
    http::server::reply reply;

};

#endif