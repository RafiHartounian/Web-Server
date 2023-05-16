#include "handler_factories/static_handler_factory.h"
#include "request_static_handler.h"

static_handler_factory::static_handler_factory(std::string location, path root_file_path) : 
    location_(location), root_file_path_(root_file_path){}

request_handler* static_handler_factory::create(std::string location, std::string request_url)
{
  return new request_static_handler(location, root_file_path_.root, request_url);//maybe it should be root_file_path_.info_map["root"]
}