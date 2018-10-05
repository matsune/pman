#pragma once
#include <condition_variable>
#include <csignal>
#include <string>
#include <vector>
#include <queue>
#include "conf_parser.hpp"
#include "pid_file.hpp"
#include "program.hpp"
#include "task.hpp"
#include "util.hpp"

class Daemon {
private:
  PmanConf conf_;
  PidFile pidFile_;
  std::vector<Program> programs_;
  std::queue<Task> tasks_;

  void daemonize();
  void registerAbrt();
  void registerSigchld();
  void handleSigchld();
  Program *getProgram(int pid);
  void cleanup();
  void startProgram(Program &program);
  void startProgram(std::string name);
  void stopProgram(Program &program);
  void stopProgram(std::string name);

public:
  Daemon(PmanConf conf, std::vector<ProgramConf> programConfs);
  std::mutex mtx;

  void setup();
  int runLoop();
  void pushTask(Task t) { this->tasks_.push(t); }
  bool hasTaskId(int id);
  std::vector<Program> programs() { return this->programs_; }
  Program *getProgram(std::string name);
};
