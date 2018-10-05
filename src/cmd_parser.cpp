#include <string>
#include <iostream>
#include <sstream>
#include "defines.h"
#include "cmd_parser.hpp"

using namespace std;

void CmdParser::parse()
{
  if (!consume()) return;

  if (isVersion()) {
    showVersion();
    exit(0);
  }

  if (isHelp()) {
    cout << usage() << endl;
    exit(0);
  }

  if (isCommand()) {
    parseCommand();
    if (!consume()) return;
  }

  parseArgs();
}

bool CmdParser::consume()
{
  if (hasNext()) {
   cursor++;
   return true;
  }
  return false;
};

bool CmdParser::hasNext()
{
  return cursor < (argc - 1);
}

bool CmdParser::match(const char *str)
{
  return strcmp(current(), str) == 0;
}

bool CmdParser::isCommand()
{
  return match("status") || match("start") || match("stop") || match("restart");
}

void CmdParser::parseCommand()
{
  assert(isCommand());

  if (match("status")) {
    this->command_ = STATUS;
  } else if (match("start")) {
    this->command_ = START;
  } else if (match("stop")) {
    this->command_ = STOP;
  } else if (match("restart")) {
    this->command_ = RESTART;
  }

  // parse program name

  // `start` and `stop` command requires program name.
  // `status` command allows empty, then called `status all` by default.
  if (!hasNext()) {
    switch (command()) {
      case START:
      case STOP:
      case RESTART:
        cerr << "requires program name" << endl << endl;
        cerr << usage() << endl;
        exit(1);
      default:
        return;
    }
  }

  consume();

  if (isArgs()) {
    if (command() == START || command() == STOP || command() == RESTART) {
      cerr << "requires program name" << endl;
      exit(1);
    } else {
      back();
      return;
    }
  }

  this->program_ = string(current());
}

bool CmdParser::isArgs()
{
  return isConf();
}

bool CmdParser::isConf()
{
  return match("--conf") || match("-c");
}

void CmdParser::parseArgs()
{
  for (;;) {
    if (isConf()) {
      if (!consume()) {
        cerr << "please pass conf file path after `--conf` or `-c`" << endl;
        exit(1);
      }

      this->conffile_ = string(current());
    } else {
      cerr << "unknown option: " << current() << endl << endl;
      cerr << usage() << endl;
      exit(1);
    }

    if (!consume()) return;
  }
}

bool CmdParser::isVersion()
{
  return match("--version") || match("-v");
}

bool CmdParser::isHelp()
{
  return match("--help") || match("-h");
}

void CmdParser::showVersion()
{
  cout << "pman version " << PMAN_VERSION << endl;
}

std::string CmdParser::usage()
{
  std::ostringstream os;
  os << "usage: pman [--version | -v] [--help | -h]" << endl
    << "\t    <command> [<args>]" << endl
    << "command:" << endl
    << "  status <program>  \tshow status" << endl
    << "  status all        \tshow status of all programs" << endl
    << "  start <program>   \tstart program" << endl
    << "  start all         \tstart all programs which is not running" << endl
    << "  stop <program>    \tstop program" << endl
    << "  stop all          \tstop all programs which is running" << endl
    << "  restart <program> \tstop and start program" << endl
    << "  restart all       \trestart all programs" << endl
    << endl
    << "args:" << endl
    << "  --conf, -c <path>\tconfig file path (default: /etc/pman.conf)" << endl;
  return os.str();
}
