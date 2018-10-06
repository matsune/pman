#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include "conf.hpp"

void Conf::setRedirect(std::string file, int to)
{
  int fd = open(file.c_str(), O_RDWR|O_CREAT|O_APPEND, 0600);
  if (fd == to) return;
  close(to);
  dup2(fd, to);
  close(fd);
}

void PmanConf::setLogfile()
{
  Conf::setRedirect(logfile_, 1);
  Conf::setRedirect(logfile_, 2);
}

int PmanConf::chdir()
{
  return ::chdir(dir_.c_str());
}

void ProgramConf::setLogfile()
{
  Conf::setRedirect(stdout_, 1);
  Conf::setRedirect(stderr_, 2);
}
