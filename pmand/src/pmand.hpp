#include <csignal>
#include <string>
#include <vector>

#include "pid_file.hpp"
#include "program.hpp"

class Pmand {
private:
  PidFile pidFile;
  std::string logFile;
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
  Pmand(std::string pidFileName = "pmand.pid",
        std::string logFileName = "pmand.log")
    :pidFile(PidFile(pidFileName)), logFile(logFileName) {}
  void cleanup();
  int run();
};
