#include <unistd.h>
#include "sock_client.hpp"

SockClient::SockClient(std::string sockPath)
 : sock(0), addr()
{
 sock = socket(AF_UNIX, SOCK_STREAM, 0);
 if (sock < 0) perror("socket");

 addr.sun_family = AF_UNIX;
 strcpy(addr.sun_path, sockPath.c_str());
}

int SockClient::connect()
{
  return ::connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_un));
}

int SockClient::send(std::string msg)
{
  return ::write(sock, msg.data(), msg.length() + 1);
}
