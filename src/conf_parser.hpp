#pragma once

#include <set>
#include <vector>
#include <string>
#include "../lib/inih/cpp/INIReader.h"

#define PMAN_SECTION "pman"
#define PROGRAM_PREFIX "program:"

struct PmanConf {
  std::string pidfile;
  std::string logfile;
  std::string dir;
  std::string sockfile;
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
  PmanConf pmanConf();
  std::set<std::string> programNames();
  std::vector<ProgramConf> programConfs();
};
