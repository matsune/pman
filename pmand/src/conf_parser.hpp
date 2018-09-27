#pragma once

#include <string>
#include "../lib/inih/cpp/INIReader.h"

struct PmandConf {
  std::string pidfile;
  std::string logfile;
  std::string dir;
};

class ConfParser {
private:
  INIReader reader;

public:
  ConfParser (std::string confFile)
    : reader(INIReader(confFile)) {};
  bool isParseError();
  PmandConf pmandConf();
};
