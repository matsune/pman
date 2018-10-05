#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include <unistd.h>
#include "defines.h"

bool startsWith(const std::string& s, const std::string& prefix)
{
  auto size = prefix.size();
  if (s.size() < size) return false;
  return std::equal(std::begin(prefix), std::end(prefix), std::begin(s));
}

size_t split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
  size_t pos = txt.find(ch);
  size_t initialPos = 0;
  strs.clear();

  while(pos != std::string::npos) {
    strs.push_back(txt.substr(initialPos, pos - initialPos));
    initialPos = pos + 1;
    pos = txt.find(ch, initialPos);
  }
  strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));
  return strs.size();
}

std::string nowString(const char* format)
{
  time_t t = time(0) ;
  char cstr[128] ;
  strftime(cstr, sizeof(cstr), format, localtime(&t));
  return cstr ;
}
