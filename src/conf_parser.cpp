#include <iostream>
#include "util.hpp"
#include "conf_parser.hpp"

using namespace std;

bool ConfParser::ParseError()
{
  return reader.ParseError() < 0;
}

PmanConf ConfParser::pmanConf()
{
  return PmanConf {
    reader.Get(PMAN_SECTION, "pidfile", "pman.pid"),
    reader.Get(PMAN_SECTION, "logfile", "pman.log"),
    reader.Get(PMAN_SECTION, "directory", "/tmp"),
    reader.Get(PMAN_SECTION, "port", "127.0.0.1:50010"),
  };
}

set<string> ConfParser::programNames()
{
  set<string> sections = reader.GetSections();
  set<string> res;

  int prefixLength = ((string)PROGRAM_PREFIX).length();

  // filter sections that starts with 'program:' prefix
  std::copy_if(sections.begin(), sections.end(), std::inserter(res, res.end()), [prefixLength](const string &value) {
    return startsWith(value, PROGRAM_PREFIX) && value.length() > prefixLength;
  });

  // remove prefix
  for (set<string>::iterator it = res.begin(); it != res.end(); ++it) {
    string &str = (string &)*it;
    str.erase(0, prefixLength);
  }
  return res;
}

std::vector<ProgramConf> ConfParser::programConfs()
{
  set<string> names = programNames();
  vector<ProgramConf> res;
  for (set<string>::iterator it = names.begin(); it != names.end(); ++it) {
    const string section = PROGRAM_PREFIX + *it;
    string logfile = reader.Get(section, "logfile", "/dev/null");
    string command = reader.Get(section, "command", "");
    bool autorestart = reader.GetBoolean(section, "autorestart", true);

    vector<string> commands;
    split(command, commands, ' ');
    res.push_back(ProgramConf {
      *it,
      logfile,
      commands,
      autorestart
    });
  }
  return res;
}
