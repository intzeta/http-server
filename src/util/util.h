#ifndef UTIL_H
#define UTIL_H

#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define max(a, b) ({\
  __typeof__ (a) _a = (a);\
  __typeof__ (b) _b = (b);\
  _a > _b ? _a : _b;\
})

#define min(a, b) ({\
  __typeof__ (a) _a = (a);\
  __typeof__ (b) _b = (b);\
  _a < _b ? _a : _b;\
})

#define BUFSIZE 512
#define BACKLOG 10

#endif
