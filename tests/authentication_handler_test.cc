#include <boost/asio/buffers_iterator.hpp>
#include <cstring>
#include <fstream>
#include <iostream>

#include "gtest/gtest.h"
#include "request_authentication_handler.h"

class AutheticationHandlerFixture : public :: testing::Test
{
  protected:
    bhttp::request<bhttp::dynamic_body> request_;
    bhttp::response<bhttp::dynamic_body> answer;
    user_profile profile = {0, "", "", true};
};

// Test for empty request body.
TEST_F(AutheticationHandlerFixture, emptyRequest)
{
  // Sample request to test.
  request_.target("/auth");

  std::ostringstream ostring;
  ostring << request_;
  std::string request_string = ostring.str();


  // Get the return reply struct from the handler function call.
  request_authentication_handler auth_test_handle("/auth", "test url", "test", "test", "/signup", "/login", profile);
  bhttp::status status_ = auth_test_handle.handle_request(request_, answer);

  EXPECT_TRUE(status_ == bhttp::status::not_found);
}

// Test for signup pages.
TEST_F(AutheticationHandlerFixture, signupHandler)
{
  // Sample request to test.
  request_.target("/auth/signup");

  std::ostringstream ostring;
  ostring << request_;
  std::string request_string = ostring.str();

  // Get the return reply struct from the handler function call.
  request_authentication_handler auth_test_handle("/auth", "test url", "test", "test", "/signup", "/login", profile);
  bhttp::status status_ = auth_test_handle.handle_request(request_, answer);

  std::vector<std::pair<std::string, std::string>> headers;
  for(auto const& field : answer)
  {
    std::pair<std::string, std::string> header;
    header.first = std::string(field.name_string());
    header.second = std::string(field.value());
    headers.push_back(header);
  }

  // Check reply struct correctness.
  bool success = (answer.result() == bhttp::status::ok &&
                  answer.has_content_length() &&
                  headers.at(1).first == "Content-Type" &&
                  headers.at(1).second == "text/html" &&
                  status_ == answer.result());

  EXPECT_TRUE(success);
}

// Test for good signup-submit user info.
TEST_F(AutheticationHandlerFixture, goodSignupSubmitHandler)
{
  // Sample request to test. 
  request_.target("/auth/signup-submit");
  request_.method(bhttp::verb::post);
  boost::beast::ostream(request_.body()) << "test|password|testy@testing.com";

  std::ostringstream ostring;
  ostring << request_;
  std::string request_string = ostring.str();

  // Get the return reply struct from the handler function call.
  request_authentication_handler auth_test_handle("/auth", "test url", "test", "test", "/signup", "/login", profile);
  bhttp::status status_ = auth_test_handle.handle_request(request_, answer);

  // Check reply struct correctness.
  bool success = (answer.result() == bhttp::status::ok &&
                  status_ == answer.result());

  EXPECT_TRUE(success);
}

// Test for bad signup-submit user info.
TEST_F(AutheticationHandlerFixture, badSignupSubmitHandler)
{
  // Sample request to test.
  request_.target("/auth/signup-submit");
  request_.method(bhttp::verb::post);
  boost::beast::ostream(request_.body()) << "badsignupsubmit";

  std::ostringstream ostring;
  ostring << request_;
  std::string request_string = ostring.str();

  // Get the return reply struct from the handler function call.
  bhttp::response<bhttp::dynamic_body> answer;
  request_authentication_handler auth_test_handle("/auth", "test url", "test", "test", "/signup", "/login", profile);
  bhttp::status status_ = auth_test_handle.handle_request(request_, answer);

  // Check reply struct correctness.
  bool success = (answer.result() == bhttp::status::bad_request &&
                  status_ == answer.result());

  EXPECT_TRUE(success);
}

TEST_F(AutheticationHandlerFixture, loginHandler)
{
  request_.target("/auth/login");

  std::ostringstream ostring;
  ostring << request_;
  std::string request_string = ostring.str();

  // Get the return reply struct from the handler function call.
  request_authentication_handler auth_test_handle("/auth", "test url", "test", "test", "/signup", "/login", profile);
  bhttp::status status_ = auth_test_handle.handle_request(request_, answer);

  std::vector<std::pair<std::string, std::string>> headers;
  for(auto const& field : answer)
  {
    std::pair<std::string, std::string> header;
    header.first = std::string(field.name_string());
    header.second = std::string(field.value());
    headers.push_back(header);
  }

  // Check reply struct correctness.
  bool success = (answer.result() == bhttp::status::ok &&
                  answer.has_content_length() &&
                  headers.at(1).first == "Content-Type" &&
                  headers.at(1).second == "text/html" &&
                  status_ == answer.result());

  EXPECT_TRUE(success);
}