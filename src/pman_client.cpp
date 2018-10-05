#include "pman_client.hpp"

using grpc::ServerBuilder;
using grpc::Status;
using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;

using pman::StatusRequest;
using pman::StartRequest;
using pman::RestartRequest;
using pman::StopRequest;
using pman::ProgramStatusReply;
using pman::Pman;

Status PmanClient::ProgramStatus(const std::string name)
{
  ClientContext context;
  ProgramStatusReply reply;
  StatusRequest req;
  req.set_name(name);
  std::unique_ptr<ClientReader<ProgramStatusReply>> reader(
    stub_->ProgramStatus(&context, req)
  );
  while (reader->Read(&reply)) {
    printStatus(reply);
  }
  return reader->Finish();
}

Status PmanClient::StartProgram(const std::string name)
{
  ClientContext context;
  ProgramStatusReply reply;
  StartRequest req;
  req.set_name(name);
  std::unique_ptr<ClientReader<ProgramStatusReply>> reader(
    stub_->StartProgram(&context, req)
  );
  while (reader->Read(&reply)) {
    printStatus(reply);
  }
  return reader->Finish();
}

Status PmanClient::StopProgram(const std::string name)
{
  ClientContext context;
  ProgramStatusReply reply;
  StopRequest req;
  req.set_name(name);
  std::unique_ptr<ClientReader<ProgramStatusReply>> reader(
    stub_->StopProgram(&context, req)
  );
  while (reader->Read(&reply)) {
    printStatus(reply);
  }
  return reader->Finish();
}

Status PmanClient::RestartProgram(const std::string name)
{
  ClientContext context;
  ProgramStatusReply reply;
  RestartRequest req;
  req.set_name(name);
  std::unique_ptr<ClientReader<ProgramStatusReply>> reader(
    stub_->RestartProgram(&context, req)
  );
  while (reader->Read(&reply)) {
    printStatus(reply);
  }
  return reader->Finish();
}

void PmanClient::printStatus(ProgramStatusReply reply)
{
  if (reply.error().status() == ::pman::ProgramStatusReply_ErrorStatus_OK) {
    std::string state = reply.state() == pman::ProgramStatusReply_State_STOPPING ? "Stopping" : "Running";
    std::cout << "[" << reply.name() << "] status: " << state << std::endl;
  } else {
    std::cout << reply.error().message() << std::endl;
  }
}
