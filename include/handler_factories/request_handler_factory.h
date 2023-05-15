#pragma once

#include <string>
#include "request_handler.h"

class request_handler_factory
{
  public:
    virtual request_handler* create(std::string location, std::string request_url) = 0;
    virtual ~request_handler_factory() {}
};

