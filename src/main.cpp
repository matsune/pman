#include <iostream>
#include "conf_parser.hpp"
#include "pman.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
  string confFile = "pman.conf";
  ConfParser parser(confFile);
  if (parser.ParseError()) {
    cerr << "Can't load conf file." << endl;
    return 1;
  }

  Pman pman(parser.pmanConf(), parser.programConfs());
  return pman.run();
}
