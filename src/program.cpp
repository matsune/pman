#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "defines.h"
#include "program.hpp"
#include "util.hpp"

void Program::started(int pid)
{
  isRunning_ = true;
  pid_ = pid;
  time(&startTime_);
}

void Program::stopped()
{
  isRunning_ = false;
  pid_ = 0;
}

void Program::spawn()
{
  conf_.setLogfile();

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
