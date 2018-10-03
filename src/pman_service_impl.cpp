#include <grpcpp/grpcpp.h>
#include "pman.grpc.pb.h"
#include "pman_service_impl.hpp"

using grpc::Service;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;

using pman::StatusRequest;
using pman::StartRequest;
using pman::StopRequest;
using pman::ProgramStatusReply;
using pman::Pman;

void PmanServiceImpl::writeStatus(ServerWriter<ProgramStatusReply>* writer, std::string name)
{
  ProgramStatusReply reply;
  for (auto p : daemon.programs()) {
    if (name == "all" || p.name() == name) {
      reply.set_name(p.name());
      reply.set_status(p.isRunning() ? pman::ProgramStatusReply_Status_RUNNING : pman::ProgramStatusReply_Status_STOPPING);
      writer->Write(reply);
    }
  }
}

Status PmanServiceImpl::ProgramStatus(
  ServerContext* context,
  const StatusRequest* request,
  ServerWriter<ProgramStatusReply>* writer)
{
  std::lock_guard<std::mutex> lk(daemon.mtx);

  ProgramStatusReply reply;
  std::string name = request->name();
  if (name.empty()) name = "all";

  writeStatus(writer, name);

  return Status::OK;
}

Status PmanServiceImpl::StartProgram(
  ServerContext* context,
  const StartRequest* request,
  ServerWriter< ProgramStatusReply>* writer)
{
  std::unique_lock<std::mutex> lk(daemon.mtx);
  std::condition_variable cv;
  Task t(cv, Task::Order::START, request->name());
  daemon.pushTask(&t);
  cv.wait(lk, [&]{ return !daemon.hasTaskId(t.id); });

  writeStatus(writer, request->name());

  return Status::OK;
}

Status PmanServiceImpl::StopProgram(
  ServerContext* context,
  const StopRequest* request,
  ServerWriter< ProgramStatusReply>* writer)
{
  std::unique_lock<std::mutex> lk(daemon.mtx);
  std::condition_variable cv;
  Task t(cv, Task::Order::STOP, request->name());
  daemon.pushTask(&t);
  cv.wait(lk, [&]{ return !daemon.hasTaskId(t.id); });

  writeStatus(writer, request->name());

  return Status::OK;
}
