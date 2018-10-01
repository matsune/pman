#include <string>
#include <poll.h>
#include <sys/socket.h>
#include <sys/un.h>

class SockServer {
private:
  std::string sockPath;
  int serverSock, clientSock;
  struct sockaddr_un serverAddr, clientAddr;
  struct pollfd pfd;

public:
  SockServer (std::string sockPath);
  int poll(int sec);
  bool hasEvents();
  int accept();
  std::string read();
  int unlink();
  void close();
};
