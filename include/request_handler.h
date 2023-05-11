#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <vector>
#include "reply.h"
#include <boost/beast/http.hpp>
#include <boost/beast/core.hpp>

namespace bhttp = boost::beast::http;

class request_handler
{
  public:
    virtual bhttp::status handle_request(const bhttp::request<bhttp::dynamic_body> req, bhttp::response<bhttp::dynamic_body>& res) = 0;
    virtual ~request_handler() {}
};

#endif