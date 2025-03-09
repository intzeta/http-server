#include "response.h"



/* Current bug: corrupted memory!111*/

static int checkFile(const char *name){
  FILE *fp;

  if((fp = fopen("util/fileList", "r")) == NULL){
    fprintf(stderr, "Couldn't open the list of files!\n");
    return -1;
  }

  char line[BUFSIZE];
  char fileName[BUFSIZE];
  int len;

  while(fgets(line, BUFSIZE, fp) != NULL){

    const char *p = line;
    while(isspace(*p)) p++;

    sscanf(p, "%d %s\n", &len, fileName);

    //printf("%d\n", strncmp(fileName, name, strlen(name)));
    //printf("%s %s %d\n", fileName, name, len);
    if(strncmp(fileName, name, strlen(name)) == 0 && len >= 0) return len;
  }

  return -1;
}


/* Gets the subwebsite address from GET request */
/* idk if it's even called that but ok          */

void infoResponse(char *name, char *dest){
  sscanf(name, "%*[^/]/%[^ ]", dest);
}

void formatHeader(char *name, char *dest){
  int fileSize;

  if((fileSize = checkFile(name)) == -1){
    fprintf(stderr, "File not found!\n");
    return;
  }

  char type[128];

  sscanf(name, "%*[^.].%[^\n]", type);
  if(!strcmp(type, "ico")) strncpy(type, "x-icon", 6);
  else if(!strcmp(type, "js")) strncpy(type, "javascript", 6);

  sprintf(dest, "HTTP/1.1 200 OK\nContent-Type: text/%s\nContent-length: %d\n\n", type, fileSize);
}
