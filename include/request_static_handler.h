#ifndef REQUEST_STATIC_HANDLER_H
#define REQUEST_STATIC_HANDLER_H

#include "request_handler_interface.h"
#include <iostream>
#include "boost/filesystem.hpp"
#include <filesystem>
#include "request.h"
#include <string>

class request_static_handler : public request_handler_interface
{
  public:
    request_static_handler();
    request_static_handler(http::server::request request, std::string root);
    void set_request(http::server::request request, std::string root);
    http::server::reply get_reply();

  private:
    http::server::reply::status_type err_code;
    http::server::reply reply;
    std::string uri;
    std::string method;
    std::string extension;
    std::string root_;
};

#endif