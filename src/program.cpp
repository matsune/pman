#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "defines.h"
#include "program.hpp"
#include "util.hpp"

bool Program::tooShort()
{
  return (time(NULL) - startTime_) <= RESTART_MIN_SEC;
}

void Program::started(int pid)
{
  isRunning_ = true;
  pid_ = pid;
  ++execCount_;
  time(&startTime_);
}

void Program::stopped()
{
  isRunning_ = false;
  pid_ = 0;
}

void Program::spawn()
{
  close(0);
  close(1);
  close(2);
  int log_fd = open(conf_.stdout.c_str(), O_RDWR|O_CREAT|O_APPEND, 0600);
  dup2(log_fd, 1);
  close(log_fd);
  log_fd = open(conf_.stderr.c_str(), O_RDWR|O_CREAT|O_APPEND, 0600);
  dup2(log_fd, 2);
  close(log_fd);

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
