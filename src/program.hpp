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
  time_t startTime_;

public:
  Program(ProgramConf conf)
    : conf_(conf), isRunning_(false), pid_(0), startTime_(0) {}
  std::string name() { return conf_.name(); }
  std::vector<std::string> command() { return conf_.command(); }
  bool isRunning() { return isRunning_; }
  int pid() { return pid_; }
  bool autorestart() { return conf_.autorestart(); }
  bool autostart() { return conf_.autostart(); }
  time_t uptime() { return time(NULL) - startTime_; }
  void started(int pid);
  void stopped();
  void spawn();
};
