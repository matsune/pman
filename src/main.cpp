#include <iostream>
#include <string.h>
#include "cmd_parser.hpp"
#include "conf_parser.hpp"
#include "daemon.hpp"
#include "sock_client.hpp"

using namespace std;

int runServer(ConfParser parser)
{
  static Daemon &daemon = Daemon::getInstance();
  daemon.setup(parser.pmanConf(), parser.programConfs());
  return daemon.runLoop();
}

int runClient(ConfParser parser, Command c, string program)
{
  // SockClient client(parser.pmanConf().sockfile);
  // if (client.connect() < 0)
  //   std::cerr << "connection error: " << parser.pmanConf().sockfile << std::endl;
  //
  // switch (c) {
  //   case E_STATUS:
  //     client.send("status");
  //     break;
  //   case E_START:
  //     client.send("start " + program);
  //     break;
  //   case E_START_ALL:
  //     client.send("startAll");
  //     break;
  //   case E_STOP:
  //     client.send("stop " + program);
  //     break;
  //   case E_STOP_ALL:
  //     client.send("stopAll");
  //     break;
  //   default:
  //     break;
  // }

  return 0;
}

int main(int argc, char *argv[])
{
  CmdParser cmd(argc, argv);
  cmd.parse();

  ConfParser parser(cmd.conffile());
  if (parser.ParseError()) {
    cerr << "Can't load conf file: " << cmd.conffile() << endl;
    return 1;
  }

  if (cmd.command() == E_DAEMON) {
    return runServer(parser);
  } else {
    return runClient(parser, cmd.command(), cmd.program());
  }
}
