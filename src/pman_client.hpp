#pragma once
#include <string>
#include <grpcpp/grpcpp.h>
#include "pman.grpc.pb.h"

class PmanClient {
public:
  PmanClient(std::shared_ptr<::grpc::Channel> channel)
    : stub_(::pman::Pman::NewStub(channel)) {}
  PmanClient(std::string address)
    : PmanClient(grpc::CreateChannel(address, grpc::InsecureChannelCredentials())) {};

  ::grpc::Status ProgramStatus(const std::string name);
  ::grpc::Status StartProgram(const std::string name);
  ::grpc::Status StopProgram(const std::string name);
  ::grpc::Status RestartProgram(const std::string name);

private:
  std::unique_ptr<::pman::Pman::Stub> stub_;
  void printStatus(pman::ProgramStatusReply reply);
};
