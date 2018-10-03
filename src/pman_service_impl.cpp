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

Status PmanServiceImpl::ProgramStatus(
  ServerContext* context,
  const StatusRequest* request,
  ServerWriter<ProgramStatusReply>* writer)
{
  ProgramStatusReply reply;
  // - TODO: mutex lock
  for (auto p : daemon.programs()) {
    if (request->name().empty() || request->name() == "all" || p.name() == request->name()) {
      reply.set_name(p.name());
      reply.set_status(p.isRunning() ? pman::ProgramStatusReply_Status_RUNNING : pman::ProgramStatusReply_Status_STOPPING);
      writer->Write(reply);
    }
  }
  return Status::OK;
}

Status PmanServiceImpl::StartProgram(
  ServerContext* context,
  const StartRequest* request,
  ServerWriter< ProgramStatusReply>* writer)
{
  return Status::OK;
}

Status PmanServiceImpl::StopProgram(
  ServerContext* context,
  const StopRequest* request,
  ServerWriter< ProgramStatusReply>* writer)
{
  return Status::OK;
}
