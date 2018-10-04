#include <gtest/gtest.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include "../src/pid_file.hpp"

#define PID_FILE "./test/test.pid"

PidFile pidfile(PID_FILE);

TEST(pid_file_test, test)
{
  EXPECT_FALSE(std::ifstream(PID_FILE).is_open());

  int pid = getpid();

  pidfile.write();
  EXPECT_TRUE(std::ifstream(PID_FILE).is_open());
  EXPECT_EQ(pid, pidfile.read());

  pidfile.remove();
  EXPECT_FALSE(std::ifstream(PID_FILE).is_open());
}
