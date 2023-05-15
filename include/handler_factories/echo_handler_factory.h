#pragma once

#include "request_handler_factory.h"
#include "http/path.h"

class echo_handler_factory : public request_handler_factory
{
  public:
    echo_handler_factory(std::string location, path root_file_path);

    request_handler* create(std::string location, std::string request_url);

  private:
    std::string location_;
    path root_file_path_;
};
