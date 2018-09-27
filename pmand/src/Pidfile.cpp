#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <cstdio>
#include <fstream>
#include "Pidfile.h"

int Pidfile::read()
{
  std::ifstream f;
  f.open(file_name);
  if (f.fail()) {
    return 0;
  }
  int pid = 0;
  std::string str;
  getline(f, str);
  sscanf(str.data(), "%d", &pid);
  return pid;
}

bool Pidfile::check()
{
  int pid = read();

  if (!pid || pid == getpid())
    return false;

  if (kill(pid, 0) && errno == ESRCH)
    return false;

  return true;
}

void Pidfile::write()
{
  std::ofstream f(file_name);
  f << getpid();
  f.close();
}

int Pidfile::remove()
{
  return std::remove(file_name.c_str());
}
