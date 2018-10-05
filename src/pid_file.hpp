#pragma once
#include <string>

class PidFile {
private:
  std::string fileName;

public:
  PidFile (std::string fileName = "")
    : fileName(fileName) {}
  int read();
  bool check();
  void write();
  int remove();
};
