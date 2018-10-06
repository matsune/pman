#pragma once
#include <string>
#include <vector>

class Conf {
protected:
  void setRedirect(std::string file, int to);
  virtual void setLogfile() = 0;
};

class PmanConf: Conf {
private:
  std::string pidfile_;
  std::string logfile_;
  std::string dir_;
  std::string port_;

public:
  PmanConf(std::string pidfile, std::string logfile, std::string dir, std::string port)
    : pidfile_(pidfile), logfile_(logfile), dir_(dir), port_(port) {}

  void setLogfile() override;
  int chdir();
  std::string pidfile() { return pidfile_; }
  std::string port() { return port_; }
};

class ProgramConf: Conf {
private:
  std::string name_;
  std::string stdout_;
  std::string stderr_;
  std::vector<std::string> command_;
  bool autorestart_;
  bool autostart_;

public:
  ProgramConf(std::string name, std::string stdout, std::string stderr, std::vector<std::string> command, bool autorestart, bool autostart)
    : name_(name), stdout_(stdout), stderr_(stderr), command_(command), autorestart_(autorestart), autostart_(autostart) {}

  void setLogfile() override;

  std::string name() { return name_; }
  std::vector<std::string> command() { return command_; }
  bool autorestart() { return autorestart_; }
  bool autostart() { return autostart_; }
};
