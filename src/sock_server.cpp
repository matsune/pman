#include <iostream>
#include <string>
#include <poll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include "sock_server.hpp"

SockServer::SockServer(std::string sockPath)
  : sockPath(sockPath), serverSock(0), clientSock(0), serverAddr(), clientAddr(), pfd()
{
  serverSock = socket(AF_UNIX, SOCK_STREAM, 0);
  if (serverSock < 0) perror("socket");

  unlink();

  serverAddr.sun_family = AF_UNIX;
  strcpy(serverAddr.sun_path, sockPath.c_str());

  if (bind(serverSock, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr_un)) < 0)
    perror("bind");
  if (listen(serverSock, 5) < 0)
    perror("listen");

  pfd.fd = serverSock;
  pfd.events = POLLIN;
}

int SockServer::poll(int sec)
{
  return ::poll(&pfd, 1, sec * 1000);
}

bool SockServer::hasEvents()
{
  return pfd.revents & POLLIN;
}

int SockServer::accept()
{
  socklen_t addrlen = sizeof(struct sockaddr_un);
  return clientSock = ::accept(serverSock, (struct sockaddr *)&clientAddr, &addrlen);
}

std::string SockServer::read()
{
  memset(&clientAddr, 0, sizeof(struct sockaddr_un));
  if (accept() < 0) return "";

  char msg[255];
  int len = recv(clientSock, msg, sizeof(msg), 0);
  msg[len] = 0;
  return std::string(msg);
}

int SockServer::unlink()
{
  return ::unlink(sockPath.c_str());
}

void SockServer::close()
{
  ::close(serverSock);
  ::close(clientSock);
}
