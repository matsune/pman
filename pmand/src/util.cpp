#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include <unistd.h>
#include "defines.h"

void setRedirect(std::string filename)
{
  for (int i = getdtablesize(); i >= 0; --i) {
    close(i);
  }

  int log_fd = open(filename.c_str(), O_RDWR|O_CREAT|O_APPEND, 0600);
  if (log_fd < 0) HANDLE_ERROR("open");
  dup2(log_fd, fileno(stdout));
  dup2(log_fd, fileno(stderr));

  close(log_fd);
}

bool startsWith(const std::string& s, const std::string& prefix)
{
  auto size = prefix.size();
  if (s.size() < size) return false;
  return std::equal(std::begin(prefix), std::end(prefix), std::begin(s));
}

size_t split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
  size_t pos = txt.find( ch );
  size_t initialPos = 0;
  strs.clear();

  // Decompose statement
  while( pos != std::string::npos ) {
    strs.push_back( txt.substr( initialPos, pos - initialPos ) );
    initialPos = pos + 1;

    pos = txt.find( ch, initialPos );
  }

  // Add the last one
  strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

  return strs.size();
}
