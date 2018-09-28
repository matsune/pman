#include <iostream>
#include "util.hpp"
#include "conf_parser.hpp"

using namespace std;

bool ConfParser::ParseError()
{
  return reader.ParseError() < 0;
}

PmandConf ConfParser::pmandConf()
{
  return PmandConf {
    reader.Get(PMAND_SECTION, "pidfile", "pmand.pid"),
    reader.Get(PMAND_SECTION, "logfile", "pmand.log"),
    reader.Get(PMAND_SECTION, "directory", "/tmp")
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
    string logfile = reader.Get(PROGRAM_PREFIX + *it, "logfile", "/dev/null");
    string command = reader.Get(PROGRAM_PREFIX + *it, "command", "");

    vector<string> commands;
    split(command, commands, ' ');
    res.push_back(ProgramConf {
      *it,
      logfile,
      commands
    });
  }
  return res;
}
