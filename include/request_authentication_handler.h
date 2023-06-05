#pragma once

#include <boost/log/trivial.hpp>
#include <string>
#include "request_handler.h"

class request_authentication_handler : public request_handler
{
public:
  request_authentication_handler(std::string location, std::string request_url,std::string root_, std::string data_path, std::string signup_);
  bhttp::status handle_request(const bhttp::request<bhttp::dynamic_body> req, bhttp::response<bhttp::dynamic_body>& res);

private:
  void log_message_info(std::string res_code);

  std::string location_;
  std::string request_url_;
  std::string root_;
  std::string data_path_;
  std::string signup_;
  reply rep;
};
