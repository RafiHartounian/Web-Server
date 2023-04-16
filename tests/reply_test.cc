#include "reply.h"
#include "gtest/gtest.h"

class ReplyTest : public ::testing::Test {
protected:
  http::server::reply reply_;
  std::vector<boost::asio::const_buffer> buffers;
};

TEST_F(ReplyTest, OkReply) {
  reply_ = http::server::reply::stock_reply(http::server::reply::ok);
  bool success_1 = reply_.status == http::server::reply::ok &&
                   reply_.content == "" && reply_.headers.size() == 2;
  bool success_2 =
      reply_.headers[0].name == "Content-Length" &&
      reply_.headers[0].value == std::to_string(reply_.content.size()) &&
      reply_.headers[1].name == "Content-Type" &&
      reply_.headers[1].value == "text/html";
  buffers = reply_.to_buffers();
  EXPECT_TRUE(success_1 && success_2);
}

TEST_F(ReplyTest, CreatedReply) {
  reply_ = http::server::reply::stock_reply(http::server::reply::created);
  bool success_1 = reply_.status == http::server::reply::created &&
                   reply_.content == "<html>"
                                     "<head><title>Created</title></head>"
                                     "<body><h1>201 Created</h1></body>"
                                     "</html>" &&
                   reply_.headers.size() == 2;
  bool success_2 =
      reply_.headers[0].name == "Content-Length" &&
      reply_.headers[0].value == std::to_string(reply_.content.size()) &&
      reply_.headers[1].name == "Content-Type" &&
      reply_.headers[1].value == "text/html";
  buffers = reply_.to_buffers();
  EXPECT_TRUE(success_1 && success_2);
}
