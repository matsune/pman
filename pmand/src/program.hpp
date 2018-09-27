#include <string>
#include <vector>

class Program {
private:
  std::string name_;
  std::vector<std::string> command_;
  bool isRunning_;
  int pid_;

public:
  Program(): name_(""), command_({}), isRunning_(false) {};
  Program(std::string name, std::vector<std::string> command)
    : name_(name), command_(command), isRunning_(false) {};
  std::string name() { return this->name_; };
  std::vector<std::string> command() { return this->command_; };
  bool isRunning() { return this->isRunning_; };
  void isRunning(bool isRunning_) { this->isRunning_ = isRunning_; };
  int pid() { return this->pid_; };
  void pid(int id) { this->pid_ = id; };
};
