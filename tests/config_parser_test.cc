#include "config_parser.h"
#include "gtest/gtest.h"

class NginxConfigParserTest : public ::testing::Test
{
protected:
  void SetUp() override { return; }
  NginxConfigParser parser;
  NginxConfig out_config;
};

TEST_F(NginxConfigParserTest, SimpleConfig)
{
  bool success = parser.Parse("example_config", &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, CommentConfig)
{
  bool success = parser.Parse("example_config_2", &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, EmptyConfig)
{
  bool success = parser.Parse("example_config_3", &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, MultipleConfig)
{
  bool success = parser.Parse("example_config_4", &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, NestedConfig)
{
  bool success = parser.Parse("example_config_5", &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, BackSlashConfig)
{
  bool success = parser.Parse("example_config_6", &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, MissingBlockConfig)
{
  bool fail = parser.Parse("example_config_7", &out_config);
  EXPECT_FALSE(fail);
}

TEST_F(NginxConfigParserTest, DoubleSemiColonConfig)
{
  bool fail = parser.Parse("example_config_8", &out_config);
  EXPECT_FALSE(fail);
}

TEST_F(NginxConfigParserTest, MissingNameBlockConfig)
{
  bool fail = parser.Parse("example_config_9", &out_config);
  EXPECT_FALSE(fail);
}

TEST_F(NginxConfigParserTest, MissingSemiColonConfig)
{
  bool fail = parser.Parse("example_config_10", &out_config);
  EXPECT_FALSE(fail);
}

TEST_F(NginxConfigParserTest, MissingClosingBraceConfig)
{
  bool fail = parser.Parse("example_config_11", &out_config);
  EXPECT_FALSE(fail);
}

TEST_F(NginxConfigParserTest, ExtraClosingBraceConfig)
{
  bool fail = parser.Parse("example_config_12", &out_config);
  EXPECT_FALSE(fail);
}

TEST_F(NginxConfigParserTest, SingleQuoteBackslashEscape)
{
  bool success = parser.Parse("example_config_13", &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, DoubleQuoteBackslashEscape)
{
  bool success = parser.Parse("example_config_14", &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, NoWhiteSpaceAfterQuote)
{
  bool fail = parser.Parse("example_config_15", &out_config);
  EXPECT_FALSE(fail);
}
