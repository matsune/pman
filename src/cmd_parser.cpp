#include <string>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <map>
#include "defines.h"
#include "cmd_parser.hpp"

using namespace std;

namespace {
  map<string, Command> commandMap = {
    {"status", STATUS},
    {"start", START},
    {"stop", STOP},
    {"restart", RESTART},
    {"kill", KILL},
    {"echo", ECHO}
  };
}

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
  return commandMap.count(current()) > 0;
}

void CmdParser::parseCommand()
{
  assert(isCommand());

  command_ = commandMap.at(current());

  // parse program name

  // `start`, `stop` and `restart` commands require program name.
  // `status` command allows empty, then called `status all` implicitly.
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

  if (isConf()) {
    if (command() == START || command() == STOP || command() == RESTART) {
      cerr << "requires program name" << endl;
      exit(1);
    } else {
      back();
      return;
    }
  } else if (isHelp()) {
    cout << usage() << endl;
    exit(0);
  }

  this->program_ = string(current());
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

string CmdParser::getCommandStr()
{
  switch (command_) {
    case DAEMON:
      return "daemon";
    case STATUS:
      return "status";
    case START:
      return "start";
    case STOP:
      return "stop";
    case RESTART:
      return "restart";
    case KILL:
      return "kill";
    case ECHO:
      return "echo";
  }
}

std::string CmdParser::usage()
{
  std::ostringstream os;
  switch (command_) {
    case DAEMON:
      os << "usage: pman [--version | -v]" << endl
        << "\t    <command> <args>" << endl
        << "command:" << endl
        << "  <no command>      \tstart daemon" << endl
        << "  status <program>  \tshow status" << endl
        << "  start <program>   \tstart program" << endl
        << "  stop <program>    \tstop program" << endl
        << "  restart <program> \tstop and start program" << endl
        << "  kill              \tkill daemon process" << endl
        << "  echo              \techo default conf" << endl
        << endl
        << "args:" << endl
        << "  --conf, -c <path>\tconfig file path (default: /etc/pman.conf)" << endl
        << "  --help, -h       \tshow help" << endl;
        break;
    case KILL:
      os << "usage: pman kill <args>" << endl
        << endl
        << "  kill running daemon process" << endl
        << endl
        << "args:" << endl
        << "  --conf, -c <path>\trunning daemon's config file path" << endl
        << "  --help, -h       \tshow help" << endl;
        break;
    case ECHO:
      os << "usage: pman echo <args>" << endl
        << endl
        << "  echo default conf" << endl
        << endl
        << "args:" << endl
        << "  --help, -h       \tshow help" << endl;
        break;
    case STATUS:
    case START:
    case STOP:
    case RESTART:
      os << "usage: pman " << getCommandStr() << " <program> <args>" << endl
        << endl
        << "  " << getCommandStr() << " <program>  \t" << getCommandStr() << " program" << endl
        << "  " << getCommandStr() << " all        \t" << getCommandStr() << " all programs" << endl
        << endl
        << "args:" << endl
        << "  --conf, -c <path>\tconfig file path (default: /etc/pman.conf)" << endl
        << "  --help, -h       \tshow help" << endl;
      break;
  }
  return os.str();
}
