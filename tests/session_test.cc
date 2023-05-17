#include "session.h"
#include "gtest/gtest.h"

class SessionFixture : public ::testing::Test
{
protected:
  boost::asio::io_service io_service;
};

// TEST_F(SessionFixture, SessionStart)
// {
//   session s(io_service);
//   EXPECT_TRUE(s.start());
// }

// TEST_F(SessionFixture, RequestBuilt)
// {
//   session s(io_service);
//   std::ostream os(&s.buf);
//   std::string example = "example\r\n\r\n";
//   std::string expected = "example\r";
//   os << example;
//   std::string result =
//     s.handle_read(boost::system::error_code(), example.size());
//   EXPECT_EQ(result, expected);
// }

// TEST_F(SessionFixture, SocketShutdown)
// {
//   session s(io_service);
//   std::ostream os(&s.buf);
//   std::string example = "example\r\n\r\n";
//   os << example;
//   std::string result =
//     s.handle_read(boost::system::error_code(), example.size());
//   boost::system::error_code err;
//   EXPECT_TRUE(s.handle_write(err));
// }

// TEST_F(SessionFixture, GetStaticEndpoint) {
//   session s(io_service);
//   std::ostream os(&s.buf);
//   std::string example = "GET /static2/don.gif HTTP/1.1\r\nHost: www.wesbite.com\r\nConnection: close\r\n\r\n";
//   os << example;
//   std::string result = s.handle_read(boost::system::error_code(), example.size());
//   std::vector<path> configured_paths = {path{endpoint_type::static_, "/static2", "../static_files/static2"}};
//   s.set_configured_paths(configured_paths);
//   path expected;
//   expected.type = endpoint_type::static_;
//   expected.endpoint = "/static2";
//   expected.root = "../static_files/static2";
//   //These ARE OKAY TO BE COMMENTED
//   //We need to fix these AFTER we add parsing
//   //EXPECT_EQ(s.get_endpoint().type, expected.type);
//   //EXPECT_EQ(s.get_endpoint().endpoint, expected.endpoint);
//   //EXPECT_EQ(s.get_endpoint().root, expected.root);
// }

// TEST_F(SessionFixture, GetEchoEndpoint) {
//   session s(io_service);
//   std::ostream os(&s.buf);
//   std::string example = "GET /echo2 HTTP/1.1\r\nHost: www.wesbite.com\r\nConnection: close\r\n\r\n";
//   os << example;
//   std::string result = s.handle_read(boost::system::error_code(), example.size());
//   std::vector<path> configured_paths = {path{endpoint_type::echo, "/echo2", ""}};
//   s.set_configured_paths(configured_paths);
//   path expected;
//   expected.type = endpoint_type::echo;
//   expected.endpoint = "/echo2";
//   expected.root = "";
//   //EXPECT_EQ(s.get_endpoint().type, expected.type);
//   //EXPECT_EQ(s.get_endpoint().endpoint, expected.endpoint);
//   //EXPECT_EQ(s.get_endpoint().root, expected.root);
// }