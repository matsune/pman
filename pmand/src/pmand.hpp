#include <csignal>
#include <string>
#include <vector>

#include "conf_parser.hpp"
#include "pid_file.hpp"
#include "program.hpp"

class Pmand {
private:
  PmandConf conf;
  PidFile pidFile;
  std::vector<Program> programs;

  void daemonize();
  void registerAbrt();
  void registerSigchld();
  void handleSigchld();
  void startProgram(Program &program);
  void startAllPrograms();
  Program *getProgram(int pid);

public:
  Pmand(PmandConf conf)
    : conf(conf), pidFile(PidFile(conf.pidfile)) {};
  Pmand(PmandConf conf, std::vector<ProgramConf> programConfs);
  void cleanup();
  int run();
};
