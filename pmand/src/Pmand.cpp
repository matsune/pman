#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "defines.h"
#include "Pmand.h"

using namespace std;

namespace {
  volatile std::sig_atomic_t abrt_status;
  volatile std::sig_atomic_t sigchld_status;
}
void abrt_handler(int signal) { abrt_status = signal; };
void sigchld_handler(int signal) { sigchld_status = signal; };

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

void Pmand::register_abrt()
{
  std::signal(SIGINT|SIGTERM|SIGQUIT, abrt_handler);
}

void Pmand::register_sigchld()
{
  struct sigaction sa;
  sa.sa_handler = &sigchld_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART|SA_NOCLDSTOP;
  if (sigaction(SIGCHLD, &sa, 0) < 0) HANDLE_ERROR("sigaction");
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
  register_abrt();
  register_sigchld();

  int child_pid;
  switch (child_pid = fork()) {
    case -1: HANDLE_ERROR("fork");
    case 0:
      sleep(10);
      exit(1);
    default:
      cout << child_pid << endl;
      break;
  }

  while (!abrt_status) {
    cout << "." << flush;
    sleep(3);

    if (sigchld_status) {
      sigchld_status = 0;
      cout << "killed child" << endl;
    }
  }

  cleanup();
  cout << "end" << endl;
  return 0;
}
