#include <iostream>
#include "conf_parser.hpp"
#include "pman.hpp"
#include "../lib/cmdline/cmdline.h"

using namespace std;

int main(int argc, char *argv[])
{
  cmdline::parser cmd;
  cmd.add<string>("conf", 'c', "pman.conf file path", false, "./pman.conf");
  cmd.parse_check(argc, argv);

  string confFile = cmd.get<string>("conf");
  ConfParser parser(confFile);
  if (parser.ParseError()) {
    cerr << "Can't load conf file." << endl;
    return 1;
  }

  Pman pman(parser.pmanConf(), parser.programConfs());
  return pman.run();
}
