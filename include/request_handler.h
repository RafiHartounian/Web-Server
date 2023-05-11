#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <vector>
#include "reply.h"

class request_handler
{
public:
  virtual http::server::reply get_reply() = 0;
  virtual ~request_handler() {}

};

#endif