#pragma once
#include <csignal>
#include <string>
#include <vector>

#include "util.hpp"
#include "conf_parser.hpp"
#include "pid_file.hpp"
#include "program.hpp"
#include "sock_server.hpp"

#define LOG std::cout << "[" << nowString() << "] "

class Daemon {
private:
  PmanConf conf_;
  PidFile pidFile_;
  std::vector<Program> programs_;

  void daemonize();
  void registerAbrt();
  void registerSigchld();
  void handleSigchld();
  Program *getProgram(int pid);
  void cleanup();

public:
  Daemon(PmanConf conf, std::vector<ProgramConf> programConfs);

  void setup();
  int runLoop();
  void startAllPrograms();
  void startProgram(Program &program);
  std::vector<Program> programs() { return this->programs_; }
};
