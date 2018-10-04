#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "../src/defines.h"
#include "../src/cmd_parser.hpp"
#include "../src/util.hpp"

class CmdParserTest: public ::testing::Test {
protected:
  int argc;
  char **argv;
  CmdParser *cmd;

  void makeArgs(std::vector<std::string> args)
  {
    argc = args.size();
    assert(argc > 0);
    argv = new char *[argc];
    for (int i = 0; i < argc; i++) {
      int size = sizeof(char) * args[i].length() + 1;
      argv[i] = new char[size];
      strcpy(argv[i], args[i].c_str());
      argv[i][size-1] = 0;
    }

    cmd = new CmdParser(argc, argv);
  }

  void parse()
  {
    cmd->parse();
  }

  virtual void TearDown()
  {
    for (int i = 0; i < argc; i++) {
      delete argv[i];
    }
    delete [] argv;
    delete cmd;
  }
};

TEST_F(CmdParserTest, NoOptions)
{
  makeArgs({ "pman" });
  parse();
  EXPECT_EQ(cmd->command(), DAEMON);
  EXPECT_EQ(cmd->conffile(), DEFAULT_CONF_PATH);
}

TEST_F(CmdParserTest, ShortVersion)
{
  makeArgs({ "pman", "-v" });
  testing::internal::CaptureStdout();
  EXPECT_EXIT(parse(), ::testing::ExitedWithCode(0), "");
  EXPECT_EQ(std::string("pman version ") + PMAN_VERSION + std::string("\n"),
            testing::internal::GetCapturedStdout());
}

TEST_F(CmdParserTest, LongVersion)
{
  makeArgs({ "pman", "--version" });
  testing::internal::CaptureStdout();
  EXPECT_EXIT(parse(), ::testing::ExitedWithCode(0), "");
  EXPECT_EQ(std::string("pman version ") + PMAN_VERSION + std::string("\n"),
            testing::internal::GetCapturedStdout());
}

TEST_F(CmdParserTest, ShortHelp)
{
  makeArgs({ "pman", "-h" });
  testing::internal::CaptureStdout();
  EXPECT_EXIT(parse(), ::testing::ExitedWithCode(0), "");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_TRUE(startsWith(output, "usage: pman"));
}

TEST_F(CmdParserTest, LongHelp)
{
  makeArgs({ "pman", "--help" });
  testing::internal::CaptureStdout();
  EXPECT_EXIT(parse(), ::testing::ExitedWithCode(0), "");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_TRUE(startsWith(output, "usage: pman"));
}

TEST_F(CmdParserTest, ShortConfOption)
{
  makeArgs({ "pman", "-c", "test.conf" });
  parse();
  EXPECT_EQ(cmd->command(), DAEMON);
  EXPECT_EQ(cmd->conffile(), "test.conf");
}

TEST_F(CmdParserTest, LongConfOption)
{
  makeArgs({ "pman", "-c", "test.conf" });
  parse();
  EXPECT_EQ(cmd->command(), DAEMON);
  EXPECT_EQ(cmd->conffile(), "test.conf");
}

TEST_F(CmdParserTest, InvalidConfOption)
{
  makeArgs({ "pman", "-c" });
  EXPECT_EXIT(parse(), ::testing::ExitedWithCode(1), "");
}

TEST_F(CmdParserTest, StatusWithNoArgs)
{
  makeArgs({ "pman", "status" });
  parse();
  EXPECT_EQ(cmd->command(), STATUS);
  EXPECT_EQ(cmd->program(), "");
  EXPECT_EQ(cmd->conffile(), DEFAULT_CONF_PATH);
}

TEST_F(CmdParserTest, StatusWithArgs)
{
  makeArgs({ "pman", "status", "all", "-c", "test.conf" });
  parse();
  EXPECT_EQ(cmd->command(), STATUS);
  EXPECT_EQ(cmd->program(), "all");
  EXPECT_EQ(cmd->conffile(), "test.conf");
}

TEST_F(CmdParserTest, StartWithArgs)
{
  makeArgs({ "pman", "start", "all" });
  parse();
  EXPECT_EQ(cmd->command(), START);
  EXPECT_EQ(cmd->program(), "all");
  EXPECT_EQ(cmd->conffile(), DEFAULT_CONF_PATH);
}

TEST_F(CmdParserTest, StartWithNoArgs)
{
  makeArgs({ "pman", "start" });
  EXPECT_EXIT(parse(), ::testing::ExitedWithCode(1), "");
}

TEST_F(CmdParserTest, StopWithArgs)
{
  makeArgs({ "pman", "stop", "all" });
  parse();
  EXPECT_EQ(cmd->command(), STOP);
  EXPECT_EQ(cmd->program(), "all");
  EXPECT_EQ(cmd->conffile(), DEFAULT_CONF_PATH);
}

TEST_F(CmdParserTest, StopWithNoArgs)
{
  makeArgs({ "pman", "stop" });
  EXPECT_EXIT(parse(), ::testing::ExitedWithCode(1), "");
}

TEST_F(CmdParserTest, UnknownOptions)
{
  makeArgs({ "pman", "-i", "aaa" });
  EXPECT_EXIT(parse(), ::testing::ExitedWithCode(1), "unknown option");
}
