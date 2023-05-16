#include "handler_factories/handler404factory.h"
#include "request_404_handler.h"


handler404factory::handler404factory(std::string location, path root_file_path) :
      location_(location), root_file_path_(root_file_path) {}

request_handler* handler404factory::create(std::string location, std::string request_url)
{
  return new request_404_handler(location, request_url);
}