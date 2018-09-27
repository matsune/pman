#include <iostream>
#include "conf_parser.hpp"
#include "pmand.hpp"

int main(int argc, char const *argv[])
{
  std::string confFile = "pmand.conf";
  ConfParser parser(confFile);
  if (parser.isParseError()) {
    std::cerr << "Can't load conf file." << std::endl;
    return 1;
  }

  Pmand pmand(parser.pmandConf());
  return pmand.run();
}
