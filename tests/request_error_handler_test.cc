#include <cstring>
#include <iostream>
#include "gtest/gtest.h"
#include "request_error_handler.h"
#include "../src/reply.cc"

class ErrorHandlerFixture : public :: testing::Test
{
  protected:
    request_error_handler handler;
};

TEST_F(ErrorHandlerFixture, NotFoundRequestTest)
{
  http::server::reply reply;
  handler.set_error_code(http::server::reply::status_type::not_found);
  reply = handler.get_reply();

  bool success = 
        reply.status == http::server::reply::status_type::not_found &&
        reply.content == http::server::stock_replies::to_string(http::server::reply::status_type::not_found) &&
        reply.headers[0].name == "Content-Length" &&
        reply.headers[0].value == std::to_string(reply.content.size()) && 
        reply.headers[1].name == "Content-Type" &&
        reply.headers[1].value == "text/html";

  EXPECT_TRUE(success);
}

TEST_F(ErrorHandlerFixture, BadRequestTest)
{
  http::server::reply reply;
  handler.set_error_code(http::server::reply::status_type::bad_request);
  reply = handler.get_reply();

  bool success = 
        reply.status == http::server::reply::status_type::bad_request &&
        reply.content == http::server::stock_replies::to_string(http::server::reply::status_type::bad_request) &&
        reply.headers[0].name == "Content-Length" &&
        reply.headers[0].value == std::to_string(reply.content.size()) && 
        reply.headers[1].name == "Content-Type" &&
        reply.headers[1].value == "text/html";
  
  EXPECT_TRUE(success);
}