#include <string>
#include <sys/socket.h>
#include <sys/un.h>

class SockClient {
private:
 int sock;
 struct sockaddr_un addr;

public:
 SockClient (std::string sockPath);
 int connect();
 int send(std::string msg);
};
