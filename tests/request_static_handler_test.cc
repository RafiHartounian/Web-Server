#include "gtest/gtest.h"
#include "request_static_handler.h"
#include "boost/filesystem.hpp"

class StaticHandlerFixture: public ::testing::Test
{
protected:
  request_static_handler handler;
  std::string root1 = "../static_files/static1";
  std::string root2 = "../static_files/static2";
  std::string root3 = "../static_files/static3";
  std::string base_uri1 = "/static1/";
  std::string base_uri2 = "/static2/";
  std::string base_uri3 = "/static3/";

};

TEST_F(StaticHandlerFixture, NormalHTMLFileResponseTest)
{
  std::string file = "example.html";
  http::server::request request;
  request.uri = base_uri1 + file;
  http::server::reply reply;
  handler.set_request(request, root1);
  reply = handler.get_reply();

  std::string full_path = root1 + "/" + file;
  std::ifstream file_(full_path.c_str(), std::ios::in | std::ios::binary);

  char c;
  std::string expected = "";
  while (file_.get(c))
  {
    expected += c;
  }
  file_.close();

  bool success =
    reply.status == http::server::reply::ok &&
    reply.content == expected &&
    reply.headers[0].name == "Content-Length" &&
    reply.headers[0].value == std::to_string(reply.content.size()) &&
    reply.headers[1].name == "Content-Type" &&
    reply.headers[1].value == "text/html";

  EXPECT_TRUE(success);
}

TEST_F(StaticHandlerFixture, NormalTxtFileResponseTest)
{
  std::string file = "example.txt";
  http::server::request request;
  request.uri = base_uri1 + file;
  http::server::reply reply;
  handler.set_request(request, root1);
  reply = handler.get_reply();

  std::string full_path = root1 + "/" + file;
  std::ifstream file_(full_path.c_str(), std::ios::in | std::ios::binary);

  char c;
  std::string expected = "";
  while (file_.get(c))
  {
    expected += c;
  }
  file_.close();

  bool success =
    reply.status == http::server::reply::ok &&
    reply.content == expected &&
    reply.headers[0].name == "Content-Length" &&
    reply.headers[0].value == std::to_string(reply.content.size()) &&
    reply.headers[1].name == "Content-Type" &&
    reply.headers[1].value == "text/plain";

  EXPECT_TRUE(success);
}

TEST_F(StaticHandlerFixture, NormalGifFileResponseTest)
{
  std::string file = "don.gif";
  http::server::request request;
  request.uri = base_uri2 + file;
  http::server::reply reply;
  handler.set_request(request, root2);
  reply = handler.get_reply();

  std::string full_path = root2 + "/" + file;
  std::ifstream file_(full_path.c_str(), std::ios::in | std::ios::binary);

  char c;
  std::string expected = "";
  while (file_.get(c))
  {
    expected += c;
  }
  file_.close();

  bool success =
    reply.status == http::server::reply::ok &&
    reply.content == expected &&
    reply.headers[0].name == "Content-Length" &&
    reply.headers[0].value == std::to_string(reply.content.size()) &&
    reply.headers[1].name == "Content-Type" &&
    reply.headers[1].value == "image/gif";

  EXPECT_TRUE(success);
}

TEST_F(StaticHandlerFixture, NormalZipFileResponseTest)
{
  std::string file = "test.zip";
  http::server::request request;
  request.uri = base_uri3 + file;
  http::server::reply reply;
  handler.set_request(request, root3);
  reply = handler.get_reply();

  std::string full_path = root3 + "/" + file;
  std::ifstream file_(full_path.c_str(), std::ios::in | std::ios::binary);

  char c;
  std::string expected = "";
  while (file_.get(c))
  {
    expected += c;
  }
  file_.close();

  bool success =
    reply.status == http::server::reply::ok &&
    reply.content == expected &&
    reply.headers[0].name == "Content-Length" &&
    reply.headers[0].value == std::to_string(reply.content.size()) &&
    reply.headers[1].name == "Content-Type" &&
    reply.headers[1].value == "application/zip";

  EXPECT_TRUE(success);
}

TEST_F(StaticHandlerFixture, BadFileResponseTest)
{
  std::string file = "non_existant.html";
  http::server::request request;
  request.uri = base_uri1 + file;
  http::server::reply reply;
  handler.set_request(request, root1);
  reply = handler.get_reply();

  bool success = reply.status == http::server::reply::not_found;

  EXPECT_TRUE(success);
}