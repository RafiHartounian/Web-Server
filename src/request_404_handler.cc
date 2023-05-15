#include <cstddef>
#include "request_404_handler.h"
#include <string>
#include <vector>

request_404_handler::request_404_handler(std::string location, std::string url) : 
    location_(location), request_url(request_url) {}

bhttp::status request_404_handler::handle_request(const bhttp::request<bhttp::dynamic_body> req, bhttp::response<bhttp::dynamic_body>& res)
{
  res.result(bhttp::status::not_found);
  boost::beast::ostream(res.body()) << rep.stock_reply(res.result_int());
  res.content_length((res.body().size()));
  res.set(bhttp::field::content_type, "text/html");
  return res.result();
}