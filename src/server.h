#ifndef SERVER_H
#define SERVER_H

#include "util/util.h"

// static void *convertStorage(struct sockaddr_storage *sast);

int setupServer(const char *port);
void loopServer(int sockfd);
void newClient(int sockfd, char *userAddr);

#endif
