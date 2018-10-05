#include <unistd.h>
#include "defines.h"
#include "program.hpp"
#include "util.hpp"

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

void Program::spawn()
{
  setRedirect(logfile());

  int count = command().size();
  char *args[count + 1];
  for (int i = 0; i < count; i++) {
    args[i] = (char *)command().at(i).c_str();
  }
  args[count] = NULL;
  execv(args[0], args);
  perror(name().c_str());
  exit(1);
}
