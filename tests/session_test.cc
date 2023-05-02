#include "session.h"
#include "gtest/gtest.h"

class SessionFixture: public ::testing::Test
{
protected:
  boost::asio::io_service io_service;
};

TEST_F(SessionFixture, SessionStart)
{
  session s(io_service);
  EXPECT_TRUE(s.start());
}

TEST_F(SessionFixture, RequestBuilt)
{
  session s(io_service);
  std::ostream os(&s.buf);
  std::string example = "example\r\n\r\n";
  std::string expected = "example\r";
  os << example;
  std::string result =
    s.handle_read(boost::system::error_code(), example.size());
  EXPECT_EQ(result, expected);
}

TEST_F(SessionFixture, SocketShutdown)
{
  session s(io_service);
  std::ostream os(&s.buf);
  std::string example = "example\r\n\r\n";
  os << example;
  std::string result =
    s.handle_read(boost::system::error_code(), example.size());
  boost::system::error_code err;
  EXPECT_TRUE(s.handle_write(err));
}
