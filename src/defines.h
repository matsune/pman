#pragma once
#include "util.hpp"

#define PMAN_VERSION "0.1"
#define HANDLE_ERROR(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define LOG std::cout << "[" << nowString() << "] "
#define DEFAULT_CONF_PATH "/etc/pman.conf"
#define RESTART_MIN_SEC 1
