#include <string>
#include <iostream>
#include "util.hpp"
#include "defines.h"
#include "cmd_parser.hpp"

using namespace std;

void CmdParser::parse()
{
  if (argc < 2) return;

  if (isVersion()) {
    showVersion();
    exit(0);
  }

  if (isHelp()) {
    cout << usage() << endl;
    exit(0);
  }

  parseOption();
  if (!parseCommand()) {
    return;
  }
  parseArgs();
}

bool CmdParser::hasNext()
{
  return cursor < (argc - 1);
}

bool CmdParser::match(const char *str)
{
  return strcmp(argv[cursor], str) == 0;
}

bool CmdParser::isOption()
{
  return isVersion() || isHelp();
}

void CmdParser::parseOption()
{
  if (!isOption()) return;

  if (isVersion()) {
    showVersion();
  } else if (isHelp()) {
    cout << usage() << endl;
  }

  exit(0);
}

bool CmdParser::isCommand()
{
  return match("status") || match("start") || match("stop");
}

// returns true if continue parsing
bool CmdParser::parseCommand()
{
  if (!isCommand()) return true;

  if (match("status")) {
    this->command_ = E_STATUS;
  } else if (match("start")) {
    this->command_ = E_START;
  } else if (match("stop")) {
    this->command_ = E_STOP;
  }

  switch (this->command_) {
    case E_START:
    case E_STOP:
      if (!hasNext()) {
        // no program name
        cerr << "requires program name" << endl << endl;
        cerr << usage() << endl;
        exit(1);
      }
      cursor++;

      if (startsWith(argv[cursor], "-")) {
        cerr << "invalid program name: " << argv[cursor] << endl;
        exit(1);
      }

      if (match("all")) {
        switch (this->command_) {
          case E_START:
            this->command_ = E_START_ALL;
            break;
          case E_STOP:
            this->command_ = E_STOP_ALL;
            break;
          default:
            break;
        }
      } else {
        this->program_ = string(argv[cursor]);
      }
      break;
    default:
      break;
  }


  if (hasNext()) {
    cursor++;
    return true;
  } else {
    return false;
  }
}

void CmdParser::parseArgs()
{
  for (;;) {
    if (match("--conf") || match("-c")) {
      parseConf();
    } else {
      cerr << "unknown option: " << argv[cursor] << endl << endl;
      cerr << usage() << endl;
      exit(1);
    }

    if (!hasNext()) break;

    cursor++;
  }
}

void CmdParser::parseConf()
{
  if (!hasNext()) {
    cerr << "please pass conf file path after `--conf` or `-c`" << endl;
    exit(1);
  }

  cursor++;
  this->conffile_ = string(argv[cursor]);
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
    << "  status          \tshow status" << endl
    << "  start <program> \tstart program" << endl
    << "  start all       \tstart all programs that not running" << endl
    << "  stop <program>  \tstop program" << endl
    << "  stop all        \tstop all programs that running" << endl
    << endl
    << "args:" << endl
    << "  --conf, -c <path>\tconfig file path" << endl;
  return os.str();
}
