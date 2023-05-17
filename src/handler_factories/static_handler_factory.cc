#include "handler_factories/static_handler_factory.h"
#include "request_static_handler.h"

static_handler_factory::static_handler_factory(std::string requested_path, NginxConfig config) :
  requested_path_(requested_path), config_(config) {}

request_handler* static_handler_factory::create(std::string location, std::string url)
{
  // TODO make it right
  return new request_static_handler(location, "", url);
}