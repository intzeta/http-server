#include "server.h"
#include "response.h"

static void *convertStorage(struct sockaddr_storage *sast){
  if(sast->ss_family == AF_INET){
    return &(((struct sockaddr_in *)sast)->sin_addr);
  }
  return &(((struct sockaddr_in6 *)sast)->sin6_addr);
}

int setupServer(const char *port){
  struct addrinfo hints, *servinfo, *p;
  int status, sockfd;

  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_family = AF_UNSPEC;
  hints.ai_flags = AI_PASSIVE;

  if((status = getaddrinfo(NULL, port, &hints, &servinfo)) != 0){
    fprintf(stderr, "Getaddrinfo failed, %d - %s\n", status, gai_strerror(status));
    sockfd = -1;
    goto endSetup;
  }

  int optval = 1;
  for(p = servinfo; p != NULL; p = p->ai_next){
    if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
      fprintf(stderr, "Socket failed!\n");
      continue;
    }

    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1){
      fprintf(stderr, "Failed to clear socket address!\n");
      sockfd = -1;
      goto endSetup;
    }

    if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1){
      close(sockfd);
      fprintf(stderr, "Failed to bind socket file descriptor!\n");
      continue;
    }

    break;
  }

  freeaddrinfo(servinfo);

  if(!p){
    fprintf(stderr, "Couldn't find socket!\n");
    sockfd = -1;
    goto endSetup;
  }

  if(listen(sockfd, BACKLOG) == -1){
    fprintf(stderr, "Error while listening - Errno: %s\n", strerror(errno));
    sockfd = -1;
    goto endSetup;
  }

endSetup:
  return sockfd;
}

/*
  1. Accept connection and create new socket fd
  2. Convert the address to readable form
  3. Create a child process so we can have multiple
     connections at once
  4. Receive request from client and put into request[BUFSIZE]
  5. Get the website file user is requesting
  6. Format the header to send with file

*/

void loopServer(int sockfd){
  struct sockaddr_storage newAddr;
  socklen_t newAddrlen = sizeof(newAddr);
  char userAddr[INET6_ADDRSTRLEN];
  char request[BUFSIZE];
  char message[BUFSIZE];
  char nameOfFile[BUFSIZE];
  char fileName[BUFSIZE];
  int newSock, status, n;

  while(1){
    if((newSock = accept(sockfd, (struct sockaddr *)&newAddr, &newAddrlen)) == -1){
      fprintf(stderr, "Couldn't accept or create new socket!\n");
      continue;
    }

    inet_ntop(newAddr.ss_family, convertStorage(&newAddr), userAddr, sizeof(userAddr));

    if(!fork()){
      close(sockfd);

      if((status = recv(newSock, request, BUFSIZE, 0)) > 0){
        //printf("Message sent by %s: \n\a", userAddr);
      }else if(status == 0){
        printf("%s has connected!\nClient disconnected!\n", userAddr);
      }else{
        fprintf(stderr, "Error receiving packets, Errno: %s\n", strerror(errno));
      }

      infoResponse(request, nameOfFile);
      //printf("GET /%s HTTP/1.1\n", nameOfFile);
      formatHeader(nameOfFile, message);

      sprintf(fileName, "../res/%s", nameOfFile);

      if(send(newSock, message, strlen(message), 0) == -1){
        fprintf(stderr, "error sending headers\n!");
      }

      memset(request, 0, BUFSIZE); // reuse the buffer to receive message

      FILE *fp = fopen(fileName, "r");
      while((n = fread(request, 1, 8192, fp)) > 0){
        send(newSock, request, n, 0);
      }

      fclose(fp);

      close(newSock);
      exit(0);
    }
    close(newSock);
  }
}
