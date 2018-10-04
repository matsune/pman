#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <string>
#include "../src/conf_parser.hpp"
#include "../src/defines.h"

#define TEST_CONF "test/test.conf"

class ConfParserTest: public ::testing::Test {
protected:
  ConfParser *parser;

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
  EXPECT_EQ(pmanConf.logfile, DAEMON_LOG_DEFAULT);
  EXPECT_EQ(pmanConf.pidfile, PID_FILE_DEFAULT);
  EXPECT_EQ(pmanConf.dir, DIR_DEFAULT);
  EXPECT_EQ(pmanConf.port, PORT_DEFAULT);
  EXPECT_TRUE(parser->programNames().empty());
  EXPECT_TRUE(parser->programConfs().empty());
}

TEST_F(ConfParserTest, ParsePmanSection)
{
  parse(
    "[pman]\n"
    "logfile=test_logfile\n"
    "pidfile=test_pidfile\n"
    "directory=./test\n"
    "port=127.0.0.1:50000\n"
  );
  EXPECT_FALSE(parser->ParseError());
  PmanConf pmanConf = parser->pmanConf();
  EXPECT_EQ(pmanConf.logfile, "test_logfile");
  EXPECT_EQ(pmanConf.pidfile, "test_pidfile");
  EXPECT_EQ(pmanConf.dir, "./test");
  EXPECT_EQ(pmanConf.port, "127.0.0.1:50000");
  EXPECT_TRUE(parser->programNames().empty());
  EXPECT_TRUE(parser->programConfs().empty());
}

TEST_F(ConfParserTest, ParseProgramSection)
{
  parse(
    "[program:test1]\n"
    "logfile=test1_logfile\n"
    "command=say hello\n"
    "autorestart=false\n"
  );
  EXPECT_FALSE(parser->ParseError());
  PmanConf pmanConf = parser->pmanConf();
  EXPECT_EQ(pmanConf.logfile, DAEMON_LOG_DEFAULT);
  EXPECT_EQ(pmanConf.pidfile, PID_FILE_DEFAULT);
  EXPECT_EQ(pmanConf.dir, DIR_DEFAULT);
  EXPECT_EQ(pmanConf.port, PORT_DEFAULT);
  ASSERT_EQ(parser->programNames().size(), 1);
  EXPECT_TRUE(parser->programNames().find("test1") != parser->programNames().end());
  ASSERT_EQ(parser->programConfs().size(), 1);
  EXPECT_EQ(parser->programConfs().at(0).logfile, "test1_logfile");
  ASSERT_EQ(parser->programConfs().at(0).command.size(), 2);
  EXPECT_EQ(parser->programConfs().at(0).command.at(0), "say");
  EXPECT_EQ(parser->programConfs().at(0).command.at(1), "hello");
  EXPECT_FALSE(parser->programConfs().at(0).autorestart);
}


TEST_F(ConfParserTest, ParseMultiplePrograms)
{
  parse(
    "[program:test1]\n"
    "logfile=test1_logfile\n"
    "command=say hello\n"
    "autorestart=false\n"
    "\n"
    "[program:test2]\n"
    "logfile=test2_logfile\n"
    "command=ls\n"
  );
  EXPECT_FALSE(parser->ParseError());
  PmanConf pmanConf = parser->pmanConf();
  EXPECT_EQ(pmanConf.logfile, DAEMON_LOG_DEFAULT);
  EXPECT_EQ(pmanConf.pidfile, PID_FILE_DEFAULT);
  EXPECT_EQ(pmanConf.dir, DIR_DEFAULT);
  EXPECT_EQ(pmanConf.port, PORT_DEFAULT);
  ASSERT_EQ(parser->programNames().size(), 2);
  EXPECT_TRUE(parser->programNames().find("test1") != parser->programNames().end());
  
  ASSERT_EQ(parser->programConfs().size(), 2);

  EXPECT_EQ(parser->programConfs().at(0).logfile, "test1_logfile");
  ASSERT_EQ(parser->programConfs().at(0).command.size(), 2);
  EXPECT_EQ(parser->programConfs().at(0).command.at(0), "say");
  EXPECT_EQ(parser->programConfs().at(0).command.at(1), "hello");
  EXPECT_FALSE(parser->programConfs().at(0).autorestart);

  EXPECT_EQ(parser->programConfs().at(1).logfile, "test2_logfile");
  ASSERT_EQ(parser->programConfs().at(1).command.size(), 1);
  EXPECT_EQ(parser->programConfs().at(1).command.at(0), "ls");
  EXPECT_TRUE(parser->programConfs().at(1).autorestart);
}
