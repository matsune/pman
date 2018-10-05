#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <cstdio>
#include <fstream>
#include "pid_file.hpp"

int PidFile::read()
{
  std::ifstream f;
  f.open(fileName);
  if (f.fail()) {
    return 0;
  }
  int pid = 0;
  std::string str;
  getline(f, str);
  sscanf(str.data(), "%d", &pid);
  f.close();
  return pid;
}

// Check if pman daemon has already been running in another pid
//
// - returns true:
//  it seems that there is no running daemon
//
// - returns false:
//  1. could not read pidfile
//  2. running daemon is myself
//  3. process of that pid doesn't exist.
bool PidFile::check()
{
  int pid = read();

  if (!pid || pid == getpid()) return false;

  if (kill(pid, 0) && errno == ESRCH) {
    remove();
    return false;
  }

  return true;
}

void PidFile::write()
{
  std::ofstream f(fileName);
  f << getpid();
  f.close();
}

int PidFile::remove()
{
  return std::remove(fileName.c_str());
}
