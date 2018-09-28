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
    reader.Get("pmand", "pidfile", "pmand.pid"),
    reader.Get("pmand", "logfile", "pmand.log"),
    reader.Get("pmand", "directory", "/tmp")
  };
}

set<string> ConfParser::programNames()
{
  set<string> sections = reader.GetSections();
  set<string> res;

  string prefix = "program:";

  // filter sections that starts with 'program:' prefix
  std::copy_if(sections.begin(), sections.end(), std::inserter(res, res.end()), [&prefix](const string &value) {
    return startsWith(value, prefix) && value.length() > prefix.length();
  });

  // remove prefix
  for (set<string>::iterator it = res.begin(); it != res.end(); ++it) {
    string &str = (string &)*it;
    str.erase(0, prefix.length());
  }
  return res;
}

std::vector<ProgramConf> ConfParser::programConfs()
{
  set<string> names = programNames();
  vector<ProgramConf> res;
  for (set<string>::iterator it = names.begin(); it != names.end(); ++it) {
    string logfile = reader.Get("programs:" + *it, "logfile", "aaa.log");
    string command = reader.Get("programs:" + *it, "command", "");
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
