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
  PmanConf conf;
  PidFile pidFile;
  std::vector<Program> programs;

  void daemonize();
  void registerAbrt();
  void registerSigchld();
  void handleSigchld();
  void startProgram(Program &program);
  Program *getProgram(int pid);
  void cleanup();

public:
  Daemon(PmanConf conf, std::vector<ProgramConf> programConfs);

  void setup();
  int runLoop();
  void startAllPrograms();
};
