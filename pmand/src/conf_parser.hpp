#pragma once

#include <set>
#include <vector>
#include <string>
#include "../lib/inih/cpp/INIReader.h"

#define PMAND_SECTION "pmand"
#define PROGRAM_PREFIX "program:"

struct PmandConf {
  std::string pidfile;
  std::string logfile;
  std::string dir;
};

struct ProgramConf {
  std::string name;
  std::string logfile;
  std::vector<std::string> command;
  bool autorestart;
};

class ConfParser {
private:
  INIReader reader;

public:
  ConfParser (std::string confFile)
    : reader(INIReader(confFile)) {}
  bool ParseError();
  PmandConf pmandConf();
  std::set<std::string> programNames();
  std::vector<ProgramConf> programConfs();
};
