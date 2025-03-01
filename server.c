#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 8192
#define BACKLOG 10

void reapChild(void){
  int saveErrno = errno;
  while(waitpid(-1, NULL, WNOHANG) > 0);
  errno = saveErrno;
}

void *convertStorageToAddr(struct sockaddr_storage *sast){
  if(sast->ss_family == AF_INET){
    return &(((struct sockaddr_in *)sast)->sin_addr);
  }
  return &(((struct sockaddr_in6 *)sast)->sin6_addr);
}

int main(int argc, char *argv[]){
  if(argc != 2){
    fprintf(stderr, "Enter port number\n");
    exit(1);
  }

  struct addrinfo hints, *servinfo, *p;
  int status, sockfd;

  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_family = AF_UNSPEC;
  hints.ai_flags = AI_PASSIVE;

  if((status = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0){
    fprintf(stderr, "Getaddrinfo failed, %d - %s\n", status, gai_strerror(status));
    exit(2);
  }

  int optval = 1;
  for(p = servinfo; p != NULL; p = p->ai_next){
    if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
      fprintf(stderr, "Socket failed!\n");
      continue;
    }

    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1){
      fprintf(stderr, "Failed to clear socket address!\n");
      exit(3);
    }

    if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1){
      close(sockfd);
      fprintf(stderr, "Failed to bind socket file descriptor!\n");
      continue;
    }

    break;
  }

  freeaddrinfo(servinfo);

  if(!p){ fprintf(stderr, "Couldn't find socket!\n"); exit(4);}
  if(listen(sockfd, BACKLOG) == -1){ fprintf(stderr, "Error while listening - Errno: %s\n", strerror(errno)); exit(5);}

  struct sockaddr_storage newAddr;
  socklen_t newAddrlen = sizeof(newAddr);
  char s[INET6_ADDRSTRLEN], buf[BUFSIZE];

  while(1){
    int newSock;
    if((newSock = accept(sockfd, (struct sockaddr *)&newAddr, &newAddrlen)) == -1){
      fprintf(stderr, "Couldn't accept or create new socket!\n");
      continue;
    }

    inet_ntop(newAddr.ss_family, convertStorageToAddr(&newAddr), s, sizeof(s));

    if(!fork()){
      close(sockfd);
      if(send(newSock, "HTTP/1.1 200 OK\nContent-Type:text/html\nContent-length: 8192\n\n", 32, 0) == -1){
        fprintf(stderr, "error sending headers\n!");
      }
      FILE *fp = fopen("index.html", "r");
      int n;
      while((n = fread(buf, 1, BUFSIZE, fp)) > 0){
        send(newSock, buf, n, 0);
      }

      if((status = recv(newSock, buf, 8192, 0)) < 0){
        fprintf(stderr, "Error receiving packets, Errno: %s\n", strerror(errno));
      }else if(status == 0){
        printf("%s has connected!\nClient disconnected!\n", s);
      }else{
        printf("%s has connected!\nMessage sent by client: %s\a\n", s, buf);
      }
      close(newSock);
      exit(0);
    }
    close(newSock);
  }

  return 0;
}
