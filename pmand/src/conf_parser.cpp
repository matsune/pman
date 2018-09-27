#include "conf_parser.hpp"

bool ConfParser::isParseError()
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
