#include "minishell.h"

int decouper(char * ligne, char * separ, char * mot[], int maxmot){
  int i;
  char *suite;

  mot[0] = strtok(ligne, separ);
  for(i = 1; mot[i - 1] != NULL; i++){
    if (i == maxmot){
      fprintf(stderr, "Erreur dans la fonction decouper: trop de mots\n");
      mot[i - 1] = NULL;
      break;
    }
    mot[i] = strtok(NULL, separ);

    if(mot[i]==NULL) return i+1;
    while(*(mot[i]+strlen(mot[i])-1)=='\\'){ //Gère les "\ " qui indique les espaces à ne pas enlever
      suite = strtok(NULL, separ);
      if(suite==NULL){
        if (i+1 == maxmot){
          fprintf(stderr, "Erreur dans la fonction decouper: trop de mots\n");
          mot[i] = NULL;
          return i+1;
        }
        mot[i+1]=suite;
        return i+2;
      }
      *(mot[i]+strlen(mot[i])-1)=' ';
      strcpy(mot[i]+strlen(mot[i]),suite);
    }
  }
  return i;
}