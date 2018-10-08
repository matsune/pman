#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include "../src/conf_parser.hpp"
#include "../src/defines.h"

#define TEST_CONF "test/test.conf"

class ConfParserTest: public ::testing::Test {
protected:
  ConfParser *parser;
  std::string currentDir;
  int stdout_ = 0;
  int stderr_ = 0;

  void parse(std::string text)
  {
    std::ofstream confFile(TEST_CONF, std::ios::trunc);
    confFile << text << std::endl;
    confFile.close();
    parser = new ConfParser(TEST_CONF);
  }

  virtual void TearDown()
  {
    delete parser;
  }

  std::string pwd()
  {
    char path[255];
  	getcwd(path, sizeof(path));
    return std::string(path);
  }

  void stashDir()
  {
    currentDir = pwd();
  }

  void popDir()
  {
    chdir(currentDir.c_str());
  }

  void stashStd()
  {
    dup2(1, stdout_);
    dup2(2, stdout_);
  }

  void popStd()
  {
    close(1);
    close(2);
    dup2(stdout_, 1);
    dup2(stderr_, 2);
    close(stdout_);
    close(stderr_);
  }

  std::string readFile(std::string file)
  {
    std::ifstream fin(file);
    std::stringstream strstream;
    strstream << fin.rdbuf();
    fin.close();
    return std::string(strstream.str());
  }

  static void TearDownTestCase()
  {
    unlink(TEST_CONF);
  }
};

TEST_F(ConfParserTest, Empty)
{
  parse("");
  EXPECT_FALSE(parser->ParseError());
  PmanConf pmanConf = parser->pmanConf();
  EXPECT_EQ(pmanConf.pidfile(), PID_FILE_DEFAULT);
  EXPECT_EQ(pmanConf.port(), PORT_DEFAULT);
  EXPECT_TRUE(parser->programNames().empty());
  EXPECT_TRUE(parser->programConfs().empty());

  std::string c = pwd();
  stashDir();
  pmanConf.chdir();
  EXPECT_EQ(pwd(), c);
  popDir();
}

TEST_F(ConfParserTest, ParsePmanSection)
{
  parse(
    "[pman]\n"
    "logfile=/tmp/test_logfile\n"
    "pidfile=/tmp/test_pidfile\n"
    "directory=/dev\n"
    "port=127.0.0.1:50000\n"
  );
  EXPECT_FALSE(parser->ParseError());
  PmanConf pmanConf = parser->pmanConf();
  EXPECT_EQ(pmanConf.pidfile(), "/tmp/test_pidfile");
  EXPECT_EQ(pmanConf.port(), "127.0.0.1:50000");
  EXPECT_TRUE(parser->programNames().empty());
  EXPECT_TRUE(parser->programConfs().empty());

  stashDir();
  EXPECT_EQ(pmanConf.chdir(), 0);
  EXPECT_EQ(pwd(), "/dev");
  popDir();

  stashStd();
  pmanConf.setLogfile();
  std::cout << "a" << std::flush;
  std::cerr << "b" << std::flush;
  popStd();

  EXPECT_EQ(readFile("/tmp/test_logfile"), "ab");
  unlink("/tmp/test_logfile");
}

TEST_F(ConfParserTest, ParseProgramSection)
{
  parse(
    "[program:test1]\n"
    "stdout=test1_stdout.log\n"
    "stderr=test1_stderr.log\n"
    "command=say hello\n"
    "autorestart=false\n"
  );
  EXPECT_FALSE(parser->ParseError());
  PmanConf pmanConf = parser->pmanConf();
  EXPECT_EQ(pmanConf.pidfile(), PID_FILE_DEFAULT);
  EXPECT_EQ(pmanConf.port(), PORT_DEFAULT);
  ASSERT_EQ(parser->programNames().size(), 1);
  EXPECT_TRUE(parser->programNames().find("test1") != parser->programNames().end());
  ASSERT_EQ(parser->programConfs().size(), 1);
  ASSERT_EQ(parser->programConfs().at(0).command().size(), 2);
  EXPECT_EQ(parser->programConfs().at(0).command().at(0), "say");
  EXPECT_EQ(parser->programConfs().at(0).command().at(1), "hello");
  EXPECT_FALSE(parser->programConfs().at(0).autorestart());

  stashStd();
  parser->programConfs().at(0).setLogfile();
  std::cout << "a" << std::flush;
  std::cerr << "b" << std::flush;
  popStd();

  EXPECT_EQ(readFile("test1_stdout.log"), "a");
  EXPECT_EQ(readFile("test1_stderr.log"), "b");
  unlink("test1_stdout.log");
  unlink("test1_stderr.log");
}

TEST_F(ConfParserTest, ParseMultiplePrograms)
{
  parse(
    "[program:test1]\n"
    "stdout=program_test1_stdout.log\n"
    "stderr=program_test1_stderr.log\n"
    "command=say hello\n"
    "autorestart=false\n"
    "\n"
    "[program:test2]\n"
    "command=ls\n"
  );
  EXPECT_FALSE(parser->ParseError());
  PmanConf pmanConf = parser->pmanConf();
  EXPECT_EQ(pmanConf.pidfile(), PID_FILE_DEFAULT);
  EXPECT_EQ(pmanConf.port(), PORT_DEFAULT);
  ASSERT_EQ(parser->programNames().size(), 2);
  EXPECT_TRUE(parser->programNames().find("test1") != parser->programNames().end());
  ASSERT_EQ(parser->programConfs().size(), 2);
  ASSERT_EQ(parser->programConfs().at(0).command().size(), 2);
  EXPECT_EQ(parser->programConfs().at(0).command().at(0), "say");
  EXPECT_EQ(parser->programConfs().at(0).command().at(1), "hello");
  EXPECT_FALSE(parser->programConfs().at(0).autorestart());
  ASSERT_EQ(parser->programConfs().at(1).command().size(), 1);
  EXPECT_EQ(parser->programConfs().at(1).command().at(0), "ls");
  EXPECT_TRUE(parser->programConfs().at(1).autorestart());

  stashStd();
  parser->programConfs().at(0).setLogfile();
  std::cout << "a" << std::flush;
  std::cerr << "b" << std::flush;
  popStd();

  EXPECT_EQ(readFile("program_test1_stdout.log"), "a");
  EXPECT_EQ(readFile("program_test1_stderr.log"), "b");
  unlink("program_test1_stdout.log");
  unlink("program_test1_stderr.log");
}
