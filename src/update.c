
#include "util/util.h"

#include <sys/stat.h>
#include <fcntl.h>
#include "dirent.h"


/*
  * Don't list the directory
  * Just the files and directory from ../res
  * so for example: /help/faq
*/

static FILE *fp;

static void dirwalk(char *dir, void (*fnp)(char *)){
  char name[BUFSIZE];
  struct dirent *dp;
  DIR *dfd;

  if((dfd = opendir(dir)) == NULL){
    fprintf(stderr, "dirwalk: %s\n", dir);
  }

  while((dp = readdir(dfd)) != NULL){
    if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;
    if(strlen(dp->d_name) + strlen(dir) + 2 > sizeof(name)){
      fprintf(stderr, "dirwalk: %s/%s", dir, dp->d_name);
    }else{
      /*
      struct stat stbuf;

      char buf[8192];
      strcat(buf, dir);
      strcat(buf, "/");
      strcat(buf, dp->d_name);

      printf("3. %s\n", buf);

      if(stat(buf, &stbuf) == -1) continue;
      if((stbuf.st_mode & S_IFMT) != S_IFDIR) printf("1. %s\n", buf);
      else printf("2. %s\n", buf);
      */

      printf("%s/%s\n", dir, dp->d_name);
      sprintf(name, "%s/%s", dir, dp->d_name);
      fflush(stdout);
      (*fnp)(name);
    }
  }
  closedir(dfd);
}

static void fname(char *name){
  struct stat stbuf;

  if(stat(name, &stbuf) == -1){
    fprintf(stderr, "fname: %s\n", name);
    return;
  }

  if((stbuf.st_mode & S_IFMT) == S_IFDIR) dirwalk(name, fname);

  //printf("%ld %s\n", stbuf.st_size, name);
}

void updateDir(char *name){
  fp = fopen("src/util/fileList", "w");
  fname(name);
  fclose(fp);
}
