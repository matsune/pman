#include <iostream>
#include "../lib/inih/cpp/INIReader.h"
#include "pmand.hpp"

int main(int argc, char const *argv[])
{
  std::string confFile = "pmand.conf";
  INIReader reader(confFile);
  if (reader.ParseError() < 0) {
    std::cerr << "Can't load conf file." << std::endl;
    return 1;
  }
  std::string pidfile = reader.Get("pmand", "pidfile", "pmand.pid");
  std::string logfile = reader.Get("pmand", "logfile", "pmand.log");
  std::string dir     = reader.Get("pmand", "directory", "/tmp");
  Pmand pmand(pidfile, logfile, dir);
  return pmand.run();
}
