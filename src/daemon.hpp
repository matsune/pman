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
  Daemon() = default;
  ~Daemon() = default;

  PmanConf conf;
  PidFile pidFile;
  std::vector<Program> programs;

  void daemonize();
  void registerAbrt();
  void registerSigchld();
  void handleSigchld();
  void startProgram(Program &program);
  void startAllPrograms();
  Program *getProgram(int pid);

public:
  Daemon(const Daemon&) = delete;
  Daemon& operator=(const Daemon&) = delete;
  Daemon(Daemon&&) = delete;
  Daemon& operator=(Daemon&&) = delete;
  static Daemon &getInstance();

  void cleanup();
  void setup(PmanConf conf, std::vector<ProgramConf> programConfs);
  int runLoop();
};
