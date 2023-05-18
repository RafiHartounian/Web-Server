#pragma once

#include "config_parser.h"
#include "request_handler_factory.h"

class api_handler_factory : public request_handler_factory
{
public:
  api_handler_factory(std::string location_, NginxConfig config);
  request_handler* create(std::string location, std::string url);

private:
  std::string location_;
  NginxConfig config_;
};