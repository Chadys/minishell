#include "minishell.h"

void cd(char*mot[],short plsr){
  int i;
  char* wd,*test;
  i=0;
  if(mot[1]!=NULL){
    if(mot[1][0]=='/')
      wd=mot[1];
    else{
      wd = getcwd(NULL,0);
      test = realloc(wd,(strlen(wd) + strlen(mot[1])+1)*sizeof(char));
      if (test==NULL){
        perror("realloc");
        free(wd);
        if(!plsr) return;
        exit(1);
      }
      strcat(wd,"/");
      strcat(wd,mot[1]);
      i=1;
    }
    if(chdir(wd))
      fprintf(stderr, "%s: %s: No such file or directory\n", mot[0],wd);
    if (i) free(wd);
  }
  if(!plsr) return;
  exit(0);
}