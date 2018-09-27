#include <csignal>
#include <string>

#include "Pidfile.h"

class Pmand {
private:
  Pidfile pid_file;
  std::string log_file;
  
  void set_redirect();
  void daemonize();
  void register_signal();

public:
  Pmand(std::string file_name = "pmand.pid",
        std::string log_file = "pmand.log")
    :pid_file(Pidfile(file_name)), log_file(log_file) {}
  void cleanup();
  int run();
};
