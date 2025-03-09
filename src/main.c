#include "server.h"

void reapChild(void){
  int saveErrno = errno;
  while(waitpid(-1, NULL, WNOHANG) > 0);
  errno = saveErrno;
}

int main(int argc, char *argv[]){
  if(argc != 2){
    fprintf(stderr, "Enter port number\n");
    exit(1);
  }

  int sockfd;
  if((sockfd = setupServer(argv[1])) < 0){
    fprintf(stderr, "Couldn't create socked file descriptor - %d", sockfd);
    exit(1);
  }

  loopServer(sockfd);
  return 0;
}
