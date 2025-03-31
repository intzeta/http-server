#include "response.h"


/* Current bug: corrupted memory!111*/

static int checkFile(char *name){
  FILE *fp;
  if(strlen(name) == 0) return -1;

  if((fp = fopen("src/util/fileList", "r")) == NULL){
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

    int maxLen = max(strlen(fileName), strlen(name));
    if(strncmp(fileName, name, maxLen) == 0 && len >= 0){
      fclose(fp);
      return len;
    }
  }

  fclose(fp);
  return -1;
}

void getFileRequest(char *name, char *dest){
  sscanf(name, "%*[^/]/%[^ ]", dest);
}

/* Gets the subwebsite address from GET request */
/* idk if it's even called that but ok          */

void formatHeader(char *name, char *dest){
  int fileSize;


  if((fileSize = checkFile(name)) == -1){
    fprintf(stderr, "File not found!\n");
    strcpy(name, "notfound.html");
    fileSize = checkFile(name);
  }

  char type[128];

  sscanf(name, "%*[^.].%[^\n]", type);
  if(!strcmp(type, "ico")) strncpy(type, "x-icon", 7);
  else if(!strcmp(type, "js")) strncpy(type, "javascript", 11);

  char buf[BUFSIZE] = {'\0'};
  strncpy(buf, name, BUFSIZE - 1);
  sprintf(name, "res/%s", buf);

  sprintf(dest, "HTTP/1.1 200 OK\nContent-Type: text/%s\nContent-length: %d\n\n", type, fileSize);
}
