#include <string>
#include <vector>
#include "conf_parser.hpp"

class Program {
private:
  ProgramConf conf;
  bool isRunning_;
  int pid_;
  int execCount_;

public:
  Program(ProgramConf conf)
    : conf(conf), isRunning_(false), pid_(0), execCount_(0) {}
  std::string logfile() { return this->conf.logfile; }
  std::string name() { return this->conf.name; }
  std::vector<std::string> command() { return this->conf.command; }
  bool isRunning() { return this->isRunning_; }
  int pid() { return this->pid_; }
  int execCount() { return this->execCount_; }
  void started(int pid);
  void stopped();
};
