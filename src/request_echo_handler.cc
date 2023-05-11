#include "request_echo_handler.h"
#include <cstddef>
#include <string>
#include <vector>

request_echo_handler::request_echo_handler(std::string location, std::string url) 
  : location_(location), request_url(url) {

}

bhttp::status request_echo_handler::handle_request(const bhttp::request<bhttp::dynamic_body> req, bhttp::response<bhttp::dynamic_body>& res)
{
  std::string req_input = req.target().to_string();
  if (location_ != req_input)
  {
    res.result(bhttp::status::not_found);
    boost::beast::ostream(res.body()) << rep.stock_reply(res.result_int());
    res.content_length((res.body().size()));
    res.set(bhttp::field::content_type, "text/html");
    return res.result();
  }
  res.result(bhttp::status::ok);
  boost::beast::ostream(res.body()) << req;
  res.content_length((res.body().size()));
  res.set(bhttp::field::content_type, "text/plain");
  return res.result();
}