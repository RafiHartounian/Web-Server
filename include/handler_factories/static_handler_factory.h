#pragma once

#include "config_parser.h"
#include "request_handler_factory.h"


class static_handler_factory : public request_handler_factory
{
public:
  static_handler_factory(std::string requested_path_, NginxConfig config);
  request_handler* create(std::string location, std::string url);

private:
  std::string requested_path_;
  NginxConfig config_;
};
