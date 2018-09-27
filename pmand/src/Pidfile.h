#include <string>

class Pidfile {
private:
  std::string file_name;

public:
  Pidfile (std::string _file_name)
    :file_name(_file_name) {};
  int read();
  bool check();
  void write();
  int remove();
};
