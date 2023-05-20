#include "handler_factories/api_handler_factory.h"
#include "request_api_handler.h"

api_handler_factory::api_handler_factory(std::string location, NginxConfig config) :
  location_(location), config_(config) {}

request_handler* api_handler_factory::create(std::string location, std::string url)
{
  return new request_api_handler(location, config_.get_root(location), url, path_counts);
}