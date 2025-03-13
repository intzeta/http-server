#ifndef RESPONSE_H
#define RESPONSE_H

#include "util/util.h"

#define infoResponse(name, dest) (sscanf(name, "%*[^/]/%[^ ]", dest))

//static int checkFile(const char *);
//void infoResponse(char *, char *); // returns file name or NULL
void formatHeader(char *, char *); //TODO

#endif
