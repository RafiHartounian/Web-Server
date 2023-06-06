#include <boost/beast.hpp>
#include <boost/log/trivial.hpp>
#include <cstddef>
#include <iostream>
#include <string>

#include "profile_manager.h"
#include "request_authentication_handler.h"


request_authentication_handler::request_authentication_handler(std::string location, std::string request_url, std::string root, std::string data_path, std::string signup, std::string login, user_profile profile)
  : location_(location), request_url_(request_url), root_(root), data_path_(data_path), signup_(signup), login_(login), profile_(profile) {
  
}


bhttp::status request_authentication_handler::handle_request(const bhttp::request<bhttp::dynamic_body> req, bhttp::response<bhttp::dynamic_body>& res)
{
  /**
   * Handle the request and check for signup
   * TODO: still need to checkout if the user is trying to login and then subsequently logout.
   */
  std::string input = req.target().to_string();
  if (location_ + login_ == input)
  {
    res.result(bhttp::status::ok);
    std::string login_page;

    std::string path = root_ + "/" + "login.html";
    fs::path filePath(path);
    if (exists(filePath) && is_regular_file(filePath))
    {
      // read file
      std::ifstream f(path);
      std::string temp_data;
      char c;
      if (f.is_open())
      {
        while (f.get(c))
        {
          temp_data += c;
        }
        f.close();
        login_page = temp_data;
      }
    }
    boost::beast::ostream(res.body()) << login_page;
    
    res.content_length((res.body().size()));
    res.set(bhttp::field::content_type, "text/html");
  }else if (location_ + signup_ == input)
  {
    res.result(bhttp::status::ok);
    std::string signup_page;

    // read in signup page
    std::string path = root_ + "/" + "signup.html";
    fs::path filePath(path);
    if (exists(filePath) && is_regular_file(filePath))
    {
      // read file
      std::ifstream f(path);
      std::string temp_data;
      char c;
      if (f.is_open())
      {
        while (f.get(c))
        {
          temp_data += c;
        }
        f.close();
        signup_page = temp_data;
      }
    }
    boost::beast::ostream(res.body()) << signup_page;
    
    res.content_length((res.body().size()));
    res.set(bhttp::field::content_type, "text/html");
  }
  // check for signup-submit sub-handler
  else if (location_ + signup_ + "-submit" == input)
  {
    std::string data = boost::beast::buffers_to_string(req.body().data());
    
    profile_manager manager(data_path_ + "/");
    // check for succeessful signup
    if(!manager.signup_string(data))
    {
      res.result(bhttp::status::bad_request);
      boost::beast::ostream(res.body()) << rep.stock_reply(res.result_int());
      res.content_length((res.body().size()));
      res.set(bhttp::field::content_type, "text/html");
    }
    else
    {
      boost::beast::ostream(res.body()) << "OK";
      res.content_length((res.body().size()));
      res.result(bhttp::status::ok);
    }
  }
  // if no matching sub-handler found, return 404
  else
  {
    res.result(bhttp::status::not_found);
    boost::beast::ostream(res.body()) << rep.stock_reply(res.result_int());
    res.content_length((res.body().size()));
    res.set(bhttp::field::content_type, "text/html");

    log_message_info("404");
  }

  return res.result();
}

user_profile request_authentication_handler::get_profile()
{
   return profile_;
}

void request_authentication_handler::log_message_info(std::string res_code)
{
  BOOST_LOG_TRIVIAL(info) << "[ResponseMetrics] "
    << "response_code: "
    << res_code
    << " "
    << "request_path: "
    << request_url_
    << " "
    << "matched_handler: authentication handler";
}