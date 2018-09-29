#pragma once

#define PMAN_VERSION "0.1"
#define HANDLE_ERROR(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
