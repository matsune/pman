#include "program.hpp"

void Program::started(int pid)
{
  this->isRunning_ = true;
  this->pid_ = pid;
  ++this->execCount_;
}

void Program::stopped()
{
  this->isRunning_ = false;
  this->pid_ = 0;
}
