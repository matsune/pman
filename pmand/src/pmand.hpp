#include <csignal>
#include <string>
#include <vector>

#include "pid_file.hpp"
#include "program.hpp"

class Pmand {
private:
  PidFile pidFile;
  std::string logFile;
  std::string dir;
  std::vector<Program *> programs;

  void redirectLogfile();
  void daemonize();
  void registerAbrt();
  void registerSigchld();
  void handleSigchld();
  void startProgram(Program *program);
  void startAllPrograms();
  Program *getProgram(int pid);

public:
  Pmand(std::string pidFile, std::string logFile, std::string dir)
    : pidFile(PidFile(pidFile)), logFile(logFile), dir(dir) {}
  void cleanup();
  int run();
};
