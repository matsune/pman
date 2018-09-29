#include <iostream>
#include <string.h>
#include "cmd_parser.hpp"
#include "conf_parser.hpp"
#include "daemon.hpp"
#include "../lib/cmdline/cmdline.h"

using namespace std;

int runServer(string conf)
{
  ConfParser parser(conf);
  if (parser.ParseError()) {
    cerr << "Can't load conf file." << endl;
    return 1;
  }

  Daemon daemon(parser.pmanConf(), parser.programConfs());
  return daemon.run();
}

int runClient(CommandType c)
{
  // - TODO:
  return 0;
}

int main(int argc, char *argv[])
{
  CmdParser cmd(argc, argv);

  if (cmd.type() == E_DAEMON) {
    return runServer(cmd.conf());
  } else {
    return runClient(cmd.type());
  }
}
