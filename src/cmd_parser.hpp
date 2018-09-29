#include <string>

enum CommandType {
  E_UNKNOWN,
  E_DAEMON,
  E_STATUS,
  E_START
};

struct Command {
  CommandType type;
  std::string name;
  std::string description;
};

class CmdParser {
private:
  CommandType type_;
  CommandType parseType(int argc, char *argv[]);
  std::string conf_;

public:
  CmdParser(int argc, char **argv);
  CommandType type() { return this->type_; }
  std::string conf() { return this->conf_; }
};
