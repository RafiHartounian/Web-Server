#ifndef REQUEST_API_HANDLER_H
#define REQUEST_API_HANDLER_H

#include "request_handler.h"

class request_api_handler : public request_handler
{
  public:
    request_api_handler(std::string location, std::string root, std::string url, std::map<std::string, int>& path_counts);
    bhttp::status handle_request(const bhttp::request<bhttp::dynamic_body> req, bhttp::response<bhttp::dynamic_body>& res);

  private:
    std::string location_;
    std::string root_; 
    std::string request_url;
    reply rep;
    std::map<std::string, int>& path_counts;

    bhttp::status handle_post(bhttp::request<bhttp::dynamic_body> req, bhttp::response<bhttp::dynamic_body>& res, std::string directory);
    bhttp::status send_bad_request(bhttp::response<bhttp::dynamic_body>& res);
};

#endif