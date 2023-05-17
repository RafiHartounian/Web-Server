#pragma once

#include "config_parser.h"
#include "request_handler_factory.h"


class echo_handler_factory : public request_handler_factory
{
public:
  echo_handler_factory(std::string requested_path, NginxConfig config);

  request_handler* create(std::string location, std::string url);

private:
  std::string requested_path_;
  NginxConfig config_;
};
