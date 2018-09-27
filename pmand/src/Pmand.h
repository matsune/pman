#include <string>

#include "Pidfile.h"

class Pmand {
private:
  Pidfile pid_file;

public:
  Pmand(std::string file_name = "pmand.pid")
    :pid_file(Pidfile(file_name)) {}
  int run();
};
