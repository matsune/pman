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

using namespace pman;

int PmanServiceImpl::currentId = 0;

void PmanServiceImpl::writeStatus(ServerWriter<ProgramStatusReply>* writer, std::string name)
{
  ProgramStatusReply reply;
  for (auto p : daemon.programs()) {
    if (name == "all" || p.name() == name) {
      reply.set_name(p.name());
      reply.set_state(p.isRunning() ? pman::ProgramStatusReply_State_RUNNING : pman::ProgramStatusReply_State_STOPPING);
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

  if (name == "all") {
    if (daemon.programs().empty()) {
      writeError(writer, ProgramStatusReply_ErrorStatus_NO_PROGRAMS, "no programs");
      return Status::OK;
    }
  } else if (!daemon.getProgram(name)) {
    writeError(writer, ProgramStatusReply_ErrorStatus_NO_SUCH_PROGRAM, "no such program");
    return Status::OK;
  }

  writeStatus(writer, name);

  return Status::OK;
}

void PmanServiceImpl::writeError(
  ServerWriter< ProgramStatusReply>* writer,
  ProgramStatusReply_ErrorStatus status,
  std::string message)
{
  ProgramStatusReply reply;
  ProgramStatusReply::Error *error = new ProgramStatusReply::Error;
  error->set_status(status);
  error->set_message(message);
  reply.set_allocated_error(error);
  writer->Write(reply);
}

Status PmanServiceImpl::StartProgram(
  ServerContext* context,
  const StartRequest* request,
  ServerWriter< ProgramStatusReply>* writer)
{
  std::unique_lock<std::mutex> lk(daemon.mtx);
  std::condition_variable cv;
  int requestId = currentId++;

  if (request->name() == "all") {
    if (daemon.programs().empty()) {
      writeError(writer, ProgramStatusReply_ErrorStatus_NO_PROGRAMS, "no programs");
      return Status::OK;
    }

    for (auto p : daemon.programs()) {
      Task t(requestId, cv, Task::Order::START, p.name());
      daemon.pushTask(t);
    }
  } else {
    Program *p = daemon.getProgram(request->name());
    if (!p) {
      writeError(writer, ProgramStatusReply_ErrorStatus_NO_SUCH_PROGRAM, "no such program");
      return Status::OK;
    } else if (p->isRunning()) {
      writeError(writer, ProgramStatusReply_ErrorStatus_ALREADY_RUNNING, "program already running");
      return Status::OK;
    }
    Task t(requestId, cv, Task::Order::START, p->name());
    daemon.pushTask(t);
  }

  // wait until completed this task
  cv.wait(lk, [&]{ return !daemon.hasTaskId(requestId); });

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
  int requestId = currentId++;

  if (request->name() == "all") {
    if (daemon.programs().empty()) {
      writeError(writer, ProgramStatusReply_ErrorStatus_NO_PROGRAMS, "no programs");
      return Status::OK;
    }

    for (auto p : daemon.programs()) {
      Task t(requestId, cv, Task::Order::STOP, p.name());
      daemon.pushTask(t);
    }
  } else {
    Program *p = daemon.getProgram(request->name());
    if (!p) {
      writeError(writer, ProgramStatusReply_ErrorStatus_NO_SUCH_PROGRAM, "no such program");
      return Status::OK;
    } else if (!p->isRunning()) {
      writeError(writer, ProgramStatusReply_ErrorStatus_ALREADY_STOPPING, "program already stopping");
      return Status::OK;
    }

    Task t(requestId, cv, Task::Order::STOP, p->name());
    daemon.pushTask(t);
  }

  cv.wait(lk, [&]{ return !daemon.hasTaskId(requestId); });

  writeStatus(writer, request->name());

  return Status::OK;
}

Status PmanServiceImpl::RestartProgram(
  ServerContext* context,
  const RestartRequest* request,
  ServerWriter< ProgramStatusReply>* writer)
{
  std::unique_lock<std::mutex> lk(daemon.mtx);
  std::condition_variable cv;
  int requestId = currentId++;

  if (request->name() == "all") {
    if (daemon.programs().empty()) {
      writeError(writer, ProgramStatusReply_ErrorStatus_NO_PROGRAMS, "no programs");
      return Status::OK;
    }

    for (auto p : daemon.programs()) {
      Task t(requestId, cv, Task::Order::RESTART, p.name());
      daemon.pushTask(t);
    }
  } else {
    Program *p = daemon.getProgram(request->name());
    if (!p) {
      writeError(writer, ProgramStatusReply_ErrorStatus_NO_SUCH_PROGRAM, "no such program");
      return Status::OK;
    }

    Task t(requestId, cv, Task::Order::RESTART, p->name());
    daemon.pushTask(t);
  }

  cv.wait(lk, [&]{ return !daemon.hasTaskId(requestId); });

  writeStatus(writer, request->name());

  return Status::OK;
}
