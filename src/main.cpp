#include <iostream>
#include <string.h>
#include <memory>
#include <thread>
#include <mutex>
#include <sys/types.h>
#include <sys/wait.h>
#include <grpcpp/grpcpp.h>
#include "cmd_parser.hpp"
#include "conf_parser.hpp"
#include "daemon.hpp"
#include "defines.h"
#include "pman_service_impl.hpp"
#include "pman_client.hpp"
#include "pid_file.hpp"

using namespace std;

void rungRPCServer(string port, Daemon &daemon)
{
  string server_address(port);
  PmanServiceImpl service(daemon);

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  unique_ptr<grpc::Server> server(builder.BuildAndStart());
  LOG << "Server listening on " << server_address << endl;

  server->Wait();
}

int runServer(PmanConf pmanConf, vector<ProgramConf> programConfs)
{
  /*
    Pman server runs 2 threads; main thread is daemon and
    sub thread is server waits for client's request.
  */
  Daemon daemon(pmanConf, programConfs);
  daemon.setup();

  LOG << "Start pman daemon..." << endl;

  thread(rungRPCServer, pmanConf.port, ref(daemon)).detach();

  return daemon.runLoop();
}

int killServer(PmanConf pmanConf)
{
  PidFile pidFile(pmanConf.pidfile);
  if (!pidFile.check()) {
    cerr << "daemon is not running." << endl;
    return 1;
  }
  int pid = pidFile.read();
  kill(pid, SIGTERM);
  waitpid(pid, 0, 0);
  pidFile.remove();
  cout << "killed pman daemon." << endl;
  return 0;
}

int runClient(string port, Command c, string program)
{
  PmanClient client(port);

  grpc::Status status;
  switch (c) {
    case STATUS:
      status = client.ProgramStatus(program);
      break;
    case START:
      status = client.StartProgram(program);
      break;
    case STOP:
      status = client.StopProgram(program);
      break;
    case RESTART:
      status = client.RestartProgram(program);
      break;
    default:
      return 0;
  }
  if (!status.ok()) {
    cout << status.error_code() << ": " << status.error_message() << endl;
  }
  return 0;
}

int main(int argc, char *argv[])
{
  CmdParser cmdParser(argc, argv);
  cmdParser.parse();

  ConfParser confParser(cmdParser.conffile());
  if (confParser.ParseError()) {
    cerr << "could not load conf file: " << cmdParser.conffile() << endl;
    return 1;
  }

  if (cmdParser.command() == DAEMON) {
    return runServer(confParser.pmanConf(), confParser.programConfs());
  } else if (cmdParser.command() == KILL) {
    return killServer(confParser.pmanConf());
  } else {
    return runClient(confParser.pmanConf().port, cmdParser.command(), cmdParser.program());
  }
}
