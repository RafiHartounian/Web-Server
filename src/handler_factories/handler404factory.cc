#include "handler_factories/handler404factory.h"
#include "request_404_handler.h"


handler404factory::handler404factory(std::string requested_path, NginxConfig config) :
  requested_path_(requested_path), config_(config) {}

request_handler* handler404factory::create(std::string location, std::string url)
{
  return new request_404_handler(location, url);
}