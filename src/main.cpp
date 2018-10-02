#include <iostream>
#include <string.h>
#include <memory>
#include <thread>
#include "cmd_parser.hpp"
#include "conf_parser.hpp"
#include "daemon.hpp"
#include "sock_client.hpp"

using namespace std;

#include "helloworld.grpc.pb.h"
#include <grpcpp/grpcpp.h>

using grpc::Service;
using grpc::ServerBuilder;
using grpc::ServerContext;

using grpc::Channel;
using grpc::ClientContext;

using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;

class GreeterServiceImpl final : public Greeter::Service {
  Status SayHello(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply) override {
    std::string prefix("Hello ");
    reply->set_message(prefix + request->name());
    return Status::OK;
  }
};

class GreeterClient {
 public:
  GreeterClient(std::shared_ptr<Channel> channel)
      : stub_(Greeter::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::string SayHello(const std::string& user) {
    // Data we are sending to the server.
    HelloRequest request;
    request.set_name(user);

    // Container for the data we expect from the server.
    HelloReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->SayHello(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      return reply.message();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

 private:
  std::unique_ptr<Greeter::Stub> stub_;
};

void serve()
{
  std::string server_address("0.0.0.0:50051");
  GreeterServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  server->Wait();
}

int runServer(ConfParser parser)
{
  static Daemon &daemon = Daemon::getInstance();
  daemon.setup(parser.pmanConf(), parser.programConfs());

  unique_ptr<std::thread> serveThread =
      unique_ptr<thread>(new thread(&serve));
  serveThread->detach();

  return daemon.runLoop();
}

int runClient(ConfParser parser, Command c, string program)
{
  GreeterClient greeter(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()));
  std::string user("world");
  std::string reply = greeter.SayHello(user);
  std::cout << "Greeter received: " << reply << std::endl;
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

  if (cmd.command() == E_DAEMON) {
    return runServer(parser);
  } else {
    return runClient(parser, cmd.command(), cmd.program());
  }
}
