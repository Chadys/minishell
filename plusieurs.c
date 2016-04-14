#include "minishell.h"

short plusieurs(char * ligne){
  size_t pauseand,pausedot;
  int tmp;
  char *test;
  list temp;

  for(pauseand=strcspn(ligne,"&"),pausedot=strcspn(ligne,";");pauseand<strlen(ligne) || pausedot<strlen(ligne);pauseand=strcspn(ligne,"&"),pausedot=strcspn(ligne,";")){
    tmp = fork();
    if (tmp < 0){
      perror("fork");
      continue;
    }
    //enfant, s'occupe de la première instruction
    if(tmp==0){
      if (pauseand<pausedot){ //si le "&" est avant
        test=strstr(ligne,"&");
        memset(test,'\0',MaxLigne-pauseand);//efface de la ligne & et tout ce qui suit
      }
      else{ //si le ";" est avant
        test=strstr(ligne,";");
        memset(test,'\0',MaxLigne-pausedot);//efface de la ligne ; et tout ce qui suit
      }
      return 1;
    }
    //parent, attend si ";" sinon passe à l'instruction d'après
    if (pausedot<pauseand){
      enfant=tmp;
      waitpid(tmp,NULL,WUNTRACED);
      enfant=0;
      memset(ligne,' ',pausedot+1);//efface de la ligne tout ce qui précède ; (inclus)
    }
    else{

      temp=malloc(sizeof(cell));
      if(temp==NULL){
        fprintf(stderr, "Too many processes in memory, please kill some\n");
        enfant=tmp;
        waitpid(tmp,NULL,WUNTRACED);
        enfant=0;
      }
      else{
        if(!enpause && !enbg)
          temp->num=1;
        else{
          if(enpause && (!enbg || enpause->num > enbg->num))
            temp->num=enpause->num+1;
          if(enbg && (!enpause || enbg->num > enpause->num))
            temp->num=enbg->num+1;
        }
        temp->pid=tmp;
        temp->next=enbg;
        enbg=temp;
        printf("\n[%d] &\n",temp->num);
        waitpid(tmp,NULL,WNOHANG);
      }
      memset(ligne,' ',pauseand+1);//efface de la ligne tout ce qui précède & (inclus)
    }
  }
  return 0;
}