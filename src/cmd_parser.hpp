#include <string>
#include "../lib/cmdline/cmdline.h"

enum Command {
  E_DAEMON,
  E_STATUS,
  E_START,
  E_START_ALL,
  E_STOP,
  E_STOP_ALL
};

class CmdParser {
private:
  int argc;
  char **argv;
  int cursor;
  Command command_;
  std::string program_;
  std::string conffile_;
  bool hasNext();
  bool match(const char *str);
  bool isOption();
  void parseOption();
  bool isCommand();
  bool parseCommand();
  void parseArgs();
  void parseConf();
  bool isVersion();
  bool isHelp();
  void showVersion();
  std::string usage();

public:
  CmdParser(int argc, char **argv)
    : argc(argc), argv(argv), cursor(1), command_(E_DAEMON), program_(""), conffile_("./pman.conf") {};
  void parse();
  Command command() { return this->command_; }
  std::string conffile() { return this->conffile_; }
  std::string program() { return this->program_; }
};
