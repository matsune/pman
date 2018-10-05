#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include "../src/program.hpp"
#include "../src/conf_parser.hpp"

class ProgramTest: public ::testing::Test {
protected:
  Program *p;

  void inject(ProgramConf conf)
  {
    p = new Program(conf);
  }

  virtual void TearDown()
  {
    delete p;
  }
};

TEST_F(ProgramTest, test)
{
  inject(ProgramConf {
    "program1",
    "program1_stdout",
    "program1_stderr",
    {"this", "is", "command"},
    true // autorestart
  });
  EXPECT_EQ(p->name(), "program1");
  ASSERT_EQ(p->command().size(), 3);
  EXPECT_EQ(p->command().at(0), "this");
  EXPECT_EQ(p->command().at(1), "is");
  EXPECT_EQ(p->command().at(2), "command");
  EXPECT_EQ(p->pid(), 0);
  EXPECT_FALSE(p->isRunning());
  EXPECT_TRUE(p->autorestart());

  int pid = 100;
  p->started(pid);
  EXPECT_EQ(p->pid(), pid);
  EXPECT_TRUE(p->isRunning());

  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_EQ(p->uptime(), 1);

  p->stopped();
  EXPECT_EQ(p->pid(), 0);
  EXPECT_FALSE(p->isRunning());
}
