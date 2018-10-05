#pragma once

#include <set>
#include <vector>
#include <string>
#include "../submodules/inih/cpp/INIReader.h"

#define PMAN_SECTION "pman"
#define PROGRAM_PREFIX "program:"

struct PmanConf {
  std::string pidfile;
  std::string logfile;
  std::string dir;
  std::string port;
};

struct ProgramConf {
  std::string name;
  std::string stdout;
  std::string stderr;
  std::vector<std::string> command;
  bool autorestart;
};

class ConfParser {
private:
  INIReader reader;
  std::string makeTmpPath(std::string program, std::string std);

public:
  ConfParser (std::string confFile)
    : reader(INIReader(confFile)) {}
  bool ParseError();
  PmanConf pmanConf();
  std::set<std::string> programNames();
  std::vector<ProgramConf> programConfs();
};
