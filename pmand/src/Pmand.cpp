#include <iostream>
#include <unistd.h>

#include "Pmand.h"

using namespace std;

int Pmand::run()
{
  if (pid_file.check()) {
    cerr << "pmand already exists" << endl;
    return 1;
  }

  pid_file.write();
  pid_file.remove();

  return 0;
}
