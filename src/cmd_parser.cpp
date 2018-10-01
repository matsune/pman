#include <string>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include "defines.h"
#include "cmd_parser.hpp"
#include "../lib/cmdline/cmdline.h"

Command commands[3] = {
  Command{
    E_DAEMON,
    "daemon",
    "start server"
  },
  Command{
    E_STATUS,
    "status",
    "show status"
  },
  Command{
    E_START,
    "start",
    "start program"
  }
};

using namespace std;

CmdParser::CmdParser(int argc, char **argv)
{
  this->type_ = parseType(argc, argv);
  cmdline::parser parser;

  if (this->type_ == E_UNKNOWN) {
    parser.add("version", 'v', "show version");
    string f = "[command]\n\ncommand:\n";
    for (auto c: commands) {
      f += "  " + c.name + "  \t" + c.description + "\n";
    }
    parser.footer(f);
    parser.parse_check(argc, argv);

    if (parser.exist("version")) {
      cout << "pman version " << PMAN_VERSION << endl;
      exit(0);
    }
    cerr << parser.usage() << endl;
    exit(1);
  }

  int new_argc = argc - 1;
  char *new_argv[new_argc];

  int len = strlen(argv[0]) + 1 + strlen(argv[1]) + 1;
  char *c = (char *)malloc(sizeof(char) * len);
  memset(c, 0, len);

  for (int i = 0; i < new_argc; i++) {
    if (i == 0) {
      strcpy(c, argv[0]);
      strcat(c, " ");
      strcat(c, argv[1]);
      new_argv[i] = c;
    } else {
      new_argv[i] = argv[i + 1];
    }
  }

  // switch (this->type_) {
  //   case E_DAEMON:
  //     break;
  //   default:
  //     break;
  // }
  parser.add<string>("conf", 'c', "conf path", false, "./pman.conf");

  parser.parse_check(new_argc, new_argv);
  free(c);

  // switch (this->type_) {
  //   case E_DAEMON:
  //     break;
  //   default:
  //     break;
  // }
  this->conf_ = parser.get<string>("conf");
}

CommandType CmdParser::parseType(int argc, char *argv[])
{
  if (argc < 2) return E_UNKNOWN;
  for (auto c : commands) {
    if (c.name == argv[1]) return c.type;
  }
  return E_UNKNOWN;
}
