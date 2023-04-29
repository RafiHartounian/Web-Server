#include <cstring>
#include "gtest/gtest.h"
#include "request_echo_handler.h"

class EchoHandlerFixture : public ::testing::Test
{
  protected:
    request_echo_handler handler;
};

TEST_F(EchoHandlerFixture, EmptyRequestTest)
{ 
  http::server::request request;
  request.original_req = "";
  handler.set_request(request, request.original_req.size());
  http::server::reply reply;
  reply = handler.get_reply();

  bool success = 
        reply.status == http::server::reply::ok &&
        reply.content == request.original_req &&
        reply.headers[0].name == "Content-Length" &&
        reply.headers[0].value == std::to_string(reply.content.size()) && 
        reply.headers[1].name == "Content-Type" &&
        reply.headers[1].value == "text/plain";

  EXPECT_TRUE(success);
}

TEST_F(EchoHandlerFixture, NormalRequestTest)
{
  http::server::request request;
  request.original_req = "test";
  handler.set_request(request, request.original_req.size());
  http::server::reply reply;
  reply = handler.get_reply();

  bool success = 
        reply.status == http::server::reply::ok &&
        reply.content == request.original_req &&
        reply.headers[0].name == "Content-Length" &&
        reply.headers[0].value == std::to_string(reply.content.size()) && 
        reply.headers[1].name == "Content-Type" &&
        reply.headers[1].value == "text/plain";

  EXPECT_TRUE(success);
}