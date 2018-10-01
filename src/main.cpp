#include <iostream>
#include <string.h>
#include "cmd_parser.hpp"
#include "conf_parser.hpp"
#include "daemon.hpp"
#include "sock_client.hpp"
#include "../lib/cmdline/cmdline.h"

using namespace std;

int runServer(ConfParser parser)
{
  Daemon daemon(parser.pmanConf(), parser.programConfs());
  return daemon.run();
}

int runClient(ConfParser parser, CommandType c)
{
  SockClient client(parser.pmanConf().sockfile);
  if (client.connect() < 0)
    std::cerr << "connection error: " << parser.pmanConf().sockfile << std::endl;

  switch (c) {
    case E_START_ALL:
      client.send("startAll");
      break;
    default:
      break;
  }

  return 0;
}

int main(int argc, char *argv[])
{
  CmdParser cmd(argc, argv);

  ConfParser parser(cmd.conf());
  if (parser.ParseError()) {
    cerr << "Can't load conf file: " << cmd.conf() << endl;
    return 1;
  }

  if (cmd.type() == E_DAEMON) {
    return runServer(parser);
  } else {
    return runClient(parser, cmd.type());
  }
}
