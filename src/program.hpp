#pragma once
#include <string>
#include <vector>
#include <time.h>
#include "conf_parser.hpp"

class Program {
private:
  ProgramConf conf_;
  bool isRunning_;
  int pid_;
  int execCount_;
  time_t startTime_;

public:
  Program(ProgramConf conf)
    : conf_(conf), isRunning_(false), pid_(0), execCount_(0), startTime_(0) {}
  std::string name() { return this->conf_.name; }
  std::vector<std::string> command() { return this->conf_.command; }
  bool isRunning() { return this->isRunning_; }
  int pid() { return this->pid_; }
  int execCount() { return this->execCount_; }
  bool autorestart() { return this->conf_.autorestart; }
  bool tooShort();
  void started(int pid);
  void stopped();
  void spawn();
};
