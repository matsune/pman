#include <gtest/gtest.h>
#include "../src/util.hpp"

TEST(util_test, uptimeString)
{
  time_t sec = 0;
  EXPECT_EQ(uptimeString(sec), "00:00:00");

  sec = 30;
  EXPECT_EQ(uptimeString(sec), "00:00:30");

  sec = 60;
  EXPECT_EQ(uptimeString(sec), "00:01:00");

  sec = 728;
  EXPECT_EQ(uptimeString(sec), "00:12:08");

  sec = 3600;
  EXPECT_EQ(uptimeString(sec), "01:00:00");

  sec = 86400;
  EXPECT_EQ(uptimeString(sec), "1day 00:00:00");

  sec = 86400 * 2 + 312;
  EXPECT_EQ(uptimeString(sec), "2days 00:05:12");
}
