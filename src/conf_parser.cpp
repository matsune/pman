#include <iostream>
#include <algorithm>
#include "defines.h"
#include "util.hpp"
#include "conf_parser.hpp"

using namespace std;

bool ConfParser::ParseError()
{
  return reader.ParseError() < 0;
}

PmanConf ConfParser::pmanConf()
{
  return PmanConf (
    reader.Get(PMAN_SECTION, PID_FILE_KEY, PID_FILE_DEFAULT),
    reader.Get(PMAN_SECTION, LOG_FILE_KEY, DAEMON_LOG_DEFAULT),
    reader.Get(PMAN_SECTION, DIR_KEY, DIR_DEFAULT),
    reader.Get(PMAN_SECTION, PORT_KEY, PORT_DEFAULT)
  );
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

string ConfParser::makeTmpPath(string program, string std)
{
  for(string::iterator it = program.begin(); it != program.end(); ++it) {
    if(*it == ' ') {
      *it = '_';
    }
  }
  return "/tmp/" + program + "_" + std + ".log";
}

vector<ProgramConf> ConfParser::programConfs()
{
  set<string> names = programNames();
  vector<ProgramConf> res;
  for (set<string>::iterator it = names.begin(); it != names.end(); ++it) {
    const string section = PROGRAM_PREFIX + *it;
    string stdout = reader.Get(section, STDOUT_KEY, makeTmpPath(*it, "stdout"));
    string stderr = reader.Get(section, STDERR_KEY, makeTmpPath(*it, "stderr"));
    string command = reader.Get(section, COMMAND_KEY, COMMAND_DEFAULT);
    bool autorestart = reader.GetBoolean(section, AUTO_RESTART_KEY, AUTO_RESTART_DEFAULT);
    bool autostart = reader.GetBoolean(section, AUTO_START_KEY, AUTO_START_DEFAULT);

    vector<string> commands;
    split(command, commands, ' ');
    res.push_back(
      ProgramConf (
        *it,
        stdout,
        stderr,
        commands,
        autorestart,
        autostart
      )
    );
  }
  return res;
}
