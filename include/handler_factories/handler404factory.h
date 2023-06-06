#pragma once

#include "config_parser.h"
#include "request_handler_factory.h"

class handler404factory : public request_handler_factory
{
public:
  handler404factory(std::string location_, NginxConfig config);

  request_handler* create(std::string location, std::string url, user_profile profile);

private:
  std::string location_;
  NginxConfig config_;
};