#pragma once
#include <csignal>
#include <string>
#include <vector>
#include <queue>
#include <condition_variable>
#include "util.hpp"
#include "conf_parser.hpp"
#include "pid_file.hpp"
#include "program.hpp"
#include "sock_server.hpp"

#define LOG std::cout << "[" << nowString() << "] "

struct Task {
  enum Operation {
    START,
    STOP
  };

  std::condition_variable &cv;
  Operation op;
  std::string name;
  Task(std::condition_variable &cv, Operation op, std::string name)
    : cv(cv), op(op), name(name) {}
};

class Daemon {
private:
  PmanConf conf_;
  PidFile pidFile_;
  std::vector<Program> programs_;
  std::queue<Task *> tasks_;

  void daemonize();
  void registerAbrt();
  void registerSigchld();
  void handleSigchld();
  Program *getProgram(std::string name);
  Program *getProgram(int pid);
  void cleanup();
  void startAllPrograms();
  void startProgram(Program &program);
  void startProgram(std::string name);
  void stopAllPrograms();
  void stopProgram(Program &program);
  void stopProgram(std::string name);

public:
  Daemon(PmanConf conf, std::vector<ProgramConf> programConfs);

  void setup();
  int runLoop();
  void pushTask(Task *t) { this->tasks_.push(t); }
  std::vector<Program> programs() { return this->programs_; }
};
