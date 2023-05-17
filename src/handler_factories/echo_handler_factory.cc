#include "handler_factories/echo_handler_factory.h"
#include "request_echo_handler.h"

echo_handler_factory::echo_handler_factory(std::string requested_path, NginxConfig config) :
  requested_path_(requested_path), config_(config) {}

request_handler* echo_handler_factory::create(std::string location, std::string url)
{
  return new request_echo_handler(location, url);
}