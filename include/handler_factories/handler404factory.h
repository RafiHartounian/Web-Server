#pragma once

#include "config_parser.h"
#include "request_handler_factory.h"

class handler404factory : public request_handler_factory
{
  public:
    handler404factory(std::string location, path root_file_path);

    request_handler* create(std::string location, std::string request_url);

  private:
    std::string location_;
    path root_file_path_;
};