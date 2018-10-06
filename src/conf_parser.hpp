#pragma once
#include <set>
#include <vector>
#include <string>
#include "../submodules/inih/cpp/INIReader.h"
#include "conf.hpp"

#define PMAN_SECTION "pman"
#define PROGRAM_PREFIX "program:"

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
