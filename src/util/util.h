#ifndef UTIL_H
#define UTIL_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "dirent.h"

#define max(a, b) (__typeof__ (a) _a = (a), _b = (b); _a > _b ? _a : _b)
#define min(a, b) (__typeof__ (a) _a = (a), _b = (b); _a < _b ? _a : _b)

#define BUFSIZE 8192
#define BACKLOG 10

#endif
