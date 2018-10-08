#pragma once
#include "util.hpp"

#define HANDLE_ERROR(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define LOG std::cout << "[" << nowString() << "] "
#define RESTART_MIN_SEC 1

#define PMAN_VERSION "0.1"
#define DEFAULT_CONF_PATH "/etc/pman.conf"

// configuration keys and default values
#define PID_FILE_KEY "pidfile"
#define PID_FILE_DEFAULT "/tmp/pman.pid"
#define LOG_FILE_KEY "logfile"
#define DAEMON_LOG_DEFAULT "/tmp/pman.log"
#define DIR_KEY "directory"
#define DIR_DEFAULT ""
#define PORT_KEY "port"
#define PORT_DEFAULT "127.0.0.1:50010"

#define STDOUT_KEY "stdout"
#define STDERR_KEY "stderr"
#define COMMAND_KEY "command"
#define COMMAND_DEFAULT ""
#define AUTO_RESTART_KEY "autorestart"
#define AUTO_RESTART_DEFAULT true
#define AUTO_START_KEY "autostart"
#define AUTO_START_DEFAULT true
