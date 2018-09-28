#include <iostream>
#include "conf_parser.hpp"
#include "pmand.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
  string confFile = "pmand.conf";
  ConfParser parser(confFile);
  if (parser.ParseError()) {
    cerr << "Can't load conf file." << endl;
    return 1;
  }

  Pmand pmand(parser.pmandConf());
  return pmand.run();
}
