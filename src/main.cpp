#include <iostream>
#include <string.h>
#include <memory>
#include <thread>
#include <mutex>
#include <grpcpp/grpcpp.h>
#include "cmd_parser.hpp"
#include "conf_parser.hpp"
#include "daemon.hpp"
#include "sock_client.hpp"
#include "pman_service_impl.hpp"
#include "pman_client.hpp"

using namespace std;

void serve(string port, Daemon &daemon)
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

int runServer(ConfParser parser)
{
  Daemon daemon(parser.pmanConf(), parser.programConfs());
  daemon.setup();
  thread(serve, parser.pmanConf().port, ref(daemon)).detach();
  return daemon.runLoop();
}

int runClient(ConfParser parser, Command c, string program)
{
  PmanClient client(parser.pmanConf().port);

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
  CmdParser cmd(argc, argv);
  cmd.parse();

  ConfParser parser(cmd.conffile());
  if (parser.ParseError()) {
    cerr << "Can't load conf file: " << cmd.conffile() << endl;
    return 1;
  }

  if (cmd.command() == DAEMON) {
    return runServer(parser);
  } else {
    return runClient(parser, cmd.command(), cmd.program());
  }
}
