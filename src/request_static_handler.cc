#include <regex>
#include "request_static_handler.h"
#include "mime_types.h"

request_static_handler::request_static_handler()
{
  err_code = http::server::reply::status_type::not_found;
  extension = "";
  uri = "";
}

request_static_handler::request_static_handler(http::server::request request, std::string root):
  uri(request.uri), method(request.method), root_(root)
{

}

void request_static_handler::set_request(http::server::request request, std::string root)
{
  uri = request.uri;
  method = request.method;
  root_ = root;
}

http::server::reply request_static_handler::get_reply()
{
  std::regex e("^(/[a-zA-Z_0-9]+)*/([a-zA-Z_0-9.]+)$");

  std::smatch match;
  std::regex_search(uri, match, e);

  std::string file_name = match.str(2);

  size_t ext_start = file_name.find_last_of(".");
  if (ext_start != std::string::npos)
  {
    extension = file_name.substr(ext_start + 1);
  }

  std::string full_path = root_ + "/" + file_name;

  boost::filesystem::path boost_path(full_path);
  if (!boost::filesystem::exists(boost_path) || !boost::filesystem::is_regular_file(full_path))
  {
    err_code = http::server::reply::status_type::not_found;
    return reply.stock_reply(err_code);
  }

  std::ifstream file_(full_path.c_str(), std::ios::in | std::ios::binary);
  if (!file_)
  {
    err_code = http::server::reply::status_type::not_found;
    return reply.stock_reply(err_code);
  }

  err_code = http::server::reply::status_type::ok;
  char c;
  std::string reply_body;
  while (file_.get(c))
  {
    reply_body += c;
  }
  file_.close();

  reply.status = http::server::reply::ok;
  reply.headers.resize(2);
  reply.content = reply_body;
  reply.headers[0].name = "Content-Length";
  reply.headers[0].value = std::to_string(reply.content.size());
  reply.headers[1].name = "Content-Type";
  reply.headers[1].value = mime_types::extension_to_type(extension);
  return reply;

}