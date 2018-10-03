#pragma once
#include <grpcpp/grpcpp.h>
#include "pman.grpc.pb.h"
#include "daemon.hpp"

class PmanServiceImpl final : public ::pman::Pman::Service {
public:
  PmanServiceImpl(Daemon &daemon) : Service(), daemon(daemon) {};

  ::grpc::Status ProgramStatus(::grpc::ServerContext* context,
    const ::pman::StatusRequest* request,
    ::grpc::ServerWriter< ::pman::ProgramStatusReply>* writer);

private:
  Daemon &daemon;
};
