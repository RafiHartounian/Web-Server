
#include "request_api_handler.h"
#include "mime_types.h"
#include <boost/log/trivial.hpp>
request_api_handler::request_api_handler(std::string location, std::string root, std::string url) :
  location_(location), root_(root), request_url(url)
{
}
bhttp::status request_api_handler::handle_request(const bhttp::request<bhttp::dynamic_body> req, bhttp::response<bhttp::dynamic_body>& res)
{
  std::string req_input = req.target().to_string();
  size_t last_slash_pos = req_input.find_last_not_of('/');
  if (last_slash_pos != std::string::npos) {
    req_input = req_input.substr(0, last_slash_pos + 1);
  }
  bhttp::verb method = req.method(); 
  switch(method) 
  {
    case bhttp::verb::post: // TODO: DELETE CODE INSIDE BLOCK ONLY ECHOES RIGHT NOW
      if (location_ != req_input)
        {
            res.result(bhttp::status::not_found);
            boost::beast::ostream(res.body()) << rep.stock_reply(res.result_int());
            res.content_length((res.body().size()));
            res.set(bhttp::field::content_type, "text/html");
            return res.result();
        }
        res.result(bhttp::status::created); // setting this one different from the others solely for testing
        boost::beast::ostream(res.body()) << req;
        res.content_length((res.body().size()));
        res.set(bhttp::field::content_type, "text/plain");
        return res.result();
      break;
    
    case bhttp::verb::get: // TODO: DELETE CODE INSIDE BLOCK, ONLY ECHOES RIGHT NOW
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
      break; 
    
    case bhttp::verb::put: // TODO: DELETE CODE INSIDE BLOCK, ONLY ECHOES RIGHT NOW
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
      break; 
    
    case bhttp::verb::delete_: // TODO: DELETE CODE INSIDE BLOCK, ONLY ECHOES RIGHT NOW
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
      break; 
    default: // Bad request 
        res.result(bhttp::status::bad_request);
        boost::beast::ostream(res.body()) << rep.stock_reply(res.result_int());
        res.content_length((res.body().size()));
        res.set(bhttp::field::content_type, "text/html");
        return res.result();
      break;  
  }
}
