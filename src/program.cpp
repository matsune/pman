#include "defines.h"
#include "program.hpp"

bool Program::tooShort()
{
  return (time(NULL) - this->startTime_) <= RESTART_MIN_SEC;
}


void Program::started(int pid)
{
  this->isRunning_ = true;
  this->pid_ = pid;
  ++this->execCount_;
  time(&this->startTime_);
}

void Program::stopped()
{
  this->isRunning_ = false;
  this->pid_ = 0;
}
