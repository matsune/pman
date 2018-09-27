#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "defines.h"
#include "Pmand.h"

using namespace std;

namespace {
  volatile std::sig_atomic_t sig_status;
}
void signal_handler(int signal) { sig_status = signal; };

void Pmand::set_redirect()
{
  for (int i = getdtablesize(); i >= 0; --i) {
    close(i);
  }

  int log_fd = open(log_file.c_str(), O_RDWR|O_CREAT|O_APPEND, 0600);
  if (log_fd < 0) HANDLE_ERROR("open");
  dup2(log_fd, fileno(stderr));
  dup2(log_fd, fileno(stdout));

  close(log_fd);
}

void Pmand::daemonize()
{
  switch (fork()) {
    case -1: HANDLE_ERROR("fork");
    case 0: break;
    default: exit(0);
  }

  umask(0);
  // - TODO: if (chdir("/") < 0) handle_error("chdir");
  if (setsid() < 0) HANDLE_ERROR("setsid");

  set_redirect();
}

void Pmand::register_signal()
{
  std::signal(SIGINT|SIGTERM|SIGQUIT, signal_handler);
}

void Pmand::cleanup()
{
  pid_file.remove();
}

int Pmand::run()
{
  if (pid_file.check()) {
    cerr << "pmand already exists" << endl;
    return 1;
  }

  daemonize();
  pid_file.write();
  register_signal();

  while (!sig_status) {
    cout << "." << flush;
    sleep(3);
  }

  cleanup();
  cout << "end" << endl;
  return 0;
}
