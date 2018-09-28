#include <string>
#include <vector>
#include "conf_parser.hpp"

class Program {
private:
  ProgramConf conf;
  bool isRunning_;
  int pid_;

public:
  Program(ProgramConf conf)
    : conf(conf), isRunning_(false), pid_(0) {};
  std::string logfile() { return this->conf.logfile; };
  std::string name() { return this->conf.name; };
  std::vector<std::string> command() { return this->conf.command; };
  bool isRunning() { return this->isRunning_; };
  void isRunning(bool isRunning_) { this->isRunning_ = isRunning_; };
  int pid() { return this->pid_; };
  void pid(int id) { this->pid_ = id; };
};
